package com.digi.ic.config;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.LinkedList;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Parser {
    

    public static void processFile(String fileName, ConfigData configData) throws IOException, NullPointerException
    {
        
        try 
        {
            initialize(fileName);
            
            lineScanner = new Scanner(new FileReader(configFile));
            
            tokenScanner = null;

            errorConfig = configData.getGroupGlobalErrors();

            token = null;
            
            /* first use a Scanner to get each word */
            while (hasToken())
            {
                if (isReadToken)
                {
                    token =  getToken();
                }
                
                if (token == null)
                {
                    /* skip new line */
                }
                else if (token.equalsIgnoreCase("globalerror"))
                {
                    NameStruct error = new NameStruct(getName(), getDescription());
                    
                    for (NameStruct err : errorConfig)
                    {
                        if (err.getName().equals(error.getName()))
                        {
                            throw new IOException("Duplicate <globalerror>: " + error.getName());
                        }
                    }
                    errorConfig.add(error);
                    isReadToken = true;
                }
                else if (token.equalsIgnoreCase("group"))
                {
                    /* syntax for parsing group:
                     *     group setting or state <name> [instances] <description>
                     */
                    groupConfig = configData.getConfigGroup(getToken());
                    
                    /* parse name */
                    String nameStr = getName();
                    
                    groupLineNumber = lineNumber;
                    
                    /* parse instances */
                    int groupInstances = 1;
                    
                    if (hasTokenInt())
                    {
                        groupInstances = getTokenInt();
                    }
                    
                    GroupStruct theGroup = new GroupStruct(nameStr, groupInstances, getDescription());

                    isReadToken = true;
                    /*
                     * Parse elements and errors for the group.
                     */
                    while (hasToken())
                    {
                        if (isReadToken)
                        {
                            token =  getToken();
                            isReadToken = false; /* token is already obtained from processElement */
                        }
                        
                        if (token.equalsIgnoreCase("element"))
                        {
                            ElementStruct element = processElement();
                            
                            element.validate();
                                    
                            theGroup.addElement(element);
                        }
                        else if (token.equalsIgnoreCase("error"))
                        {
                            NameStruct error = new NameStruct(getName(), getDescription());
                            theGroup.addError(error);
                            isReadToken = true;
                        }
                        else
                        {
                            break;
                        }
                    }
                    
                    theGroup.validate();
                    
                    for (GroupStruct g : groupConfig)
                    {
                        if (g.getName().equals(theGroup.getName()))
                        {
                            throw new Exception("Duplicate <group>: " +  theGroup.getName());
                        }
                    }
                    groupConfig.add(theGroup);
                }
                else
                {
                    throw new IOException("Unrecogized keyword: " + token);
                }
            }
        } catch (NullPointerException e) {
            ConfigGenerator.log(e.toString());
            throw new NullPointerException();

        } catch (IOException e) {
            fileErrorFoundLog(e.getMessage());
            throw new IOException("Error found in file: " + fileName);
            
        } catch (Exception e) {
            String msg = e.getMessage();
            groupErrorFoundLog(msg);
            throw new IOException("Error found in file:" + fileName);
        }
        
        finally {
          //ensure the underlying stream is always closed
          //this only has any effect if the item passed to the Scanner
          //constructor implements Closeable (which it does in this case).
          if (tokenScanner != null)
          {
              tokenScanner.close();
          }
          lineScanner.close();
        }
        
    }

    private static void fileErrorFoundLog(String str)
    {
        ConfigGenerator.log("Error found in line " + lineNumber);
        ConfigGenerator.log(str);
    }

    private static void groupErrorFoundLog(String str)
    {
        if (str.indexOf("group") != -1)
            ConfigGenerator.log("Error found in line " + groupLineNumber);
        else if (str.indexOf("element") != -1)
            ConfigGenerator.log("Error found in line " + elementLineNumber);
            
        ConfigGenerator.log(str);
    }

    private final static Pattern ALPHANUMERIC = Pattern.compile("[A-Za-z_0-9]+");
    public static boolean checkAlphaNumeric(String s)
    {
        if( s == null){ return false; }
        else
        {
            Matcher m = ALPHANUMERIC.matcher(s);
            return m.matches();
        }
    }


    public static boolean getFloatingSupport()
    {
        return floatingSupport;
    }
    
    public static boolean getEnumSupport()
    {
        return enumSupport;
    }
    
    private static void initialize(String fileName) throws NullPointerException
    {
        configFile = new File(fileName);
        
        if (!configFile.exists()) 
        {
            throw new NullPointerException(fileName + " file not found.");
        }
/*        
        if (!(configFile.isFile() && configFile.canRead())) 
        {
            throw new FileNotFoundException("Unable to access " + aFileName);
        }
*/        
        isReadToken = true;
        lineNumber = 0;
        groupLineNumber = 0;
        elementLineNumber = 0;
         
    }

    private static String getName() throws IOException 
    {
        String name = getToken(); //tokenScanner.next();
        
        if (name != null)
        {
            if (!checkAlphaNumeric(name)) 
            {
                throw new IOException("Invalid character in the name: " + name);
            }
        
        }
        return name;
    }
    
    private static String getDescription() throws IOException 
    {

        String description = null;
        if (hasToken("\\\".*"))
        {
            description = getTokenInLine("\\\".*?\\\"");
            if (description == null)
            {
                throw new IOException("Invalid description");
            }
        }
        return description;
    }
    
    private static String getType() throws IOException
    {
        String type = getToken(); //tokenScanner.next();
        
        if (type == null)
        {
            throw new IOException("Missing type");
            
        }
        ElementStruct.ElementType elementType = ElementStruct.ElementType.toElementType(type);
        
        if (elementType == ElementStruct.ElementType.INVALID_TYPE)
        {
            throw new IOException("Invalid type: " + type);
        }
        else if (elementType == ElementStruct.ElementType.ENUM)
        {
            enumSupport = true;
        }
        else if (elementType == ElementStruct.ElementType.FLOAT)
        {
            floatingSupport = true;
        }
        
        return type;
    }
    
    private static String getAccess() throws IOException
    {
        String access = getToken(); // tokenScanner.next();
        
        if (access == null)
        {
            throw new IOException("Missing access");
            
        }
        if (ElementStruct.AccessType.toAccessType(access) == ElementStruct.AccessType.INVALID_TYPE)
        {
            throw new IOException("Invalid access: " + access);
        }
        return access;
    }

    private static String getMinMax() throws IOException
    {
        String mvalue = getToken(); // tokenScanner.next();
        
        if (mvalue == null)
        {
            throw new IOException("Missing min or max value");
            
        }
        return mvalue;
    }
   

   private static final ElementStruct processElement() throws IOException
   {
       /*
        * syntax for parsing element:
        *       element <name> <description> type <type> [min <min>] [max <max>] [access <access>] [unit <unit>]
        */
       ElementStruct element = new ElementStruct(getName(), getDescription());
       elementLineNumber = lineNumber;
       try {
           
           while (hasToken())
           {
               token =  getToken();
               if (token == null)
               {
                   /* end of scanner */
               }
               else if (token.equalsIgnoreCase("type"))
               {
                   element.setType(getType());
               }
               else if (token.equalsIgnoreCase("access"))
               {
                   element.setAccess(getAccess());
               }
               else if (token.equalsIgnoreCase("min"))
               {
                   element.setMin(getMinMax());
               }
               else if (token.equalsIgnoreCase("max"))
               {
                    element.setMax(getMinMax());
                   
               }
               else if (token.equalsIgnoreCase("unit"))
               {
                   element.setUnit(getDescription());
               }
               else if (token.equalsIgnoreCase("value"))
               {
                   /* 
                    * Parse Value for element with enum type
                    * syntax for parsing value:
                    *       value <name> [description]
                    */
                   NameStruct value = new NameStruct(getName());
                   
                   if (hasToken("\\\".*"))
                   {
                       value.setDescription(getDescription());
                   }
                   element.addValue(value);
               }
               else
               {
                   break;
               }
           }
           
       } catch (IOException e) {
           throw new IOException(e.toString());
       }
       
       return element;

   }
   
   private static String getToken()
   {
       String aWord = null;
       
       if (tokenScanner != null && !tokenScanner.hasNext())
       {
           tokenScanner.close();
           tokenScanner = null;
       }
       if (tokenScanner == null)
       {
           while (lineScanner.hasNextLine())
           {
               String line = lineScanner.nextLine();
               lineNumber++;
//               ConfigGenerator.log("line " + lineNumber + ": " + line);
               
               if (line.length() > 0 && line.split(" ").length > 0)
               {
                   tokenScanner = new Scanner(line);
                   break;
               }
           }
       }
       
       if (tokenScanner != null && tokenScanner.hasNext())
           aWord = tokenScanner.next();

       return aWord;
   }
   private static int getTokenInt() throws IOException
   {
       int anInt = 0;
       
       if (tokenScanner != null && !tokenScanner.hasNext())
       {
           tokenScanner.close();
           tokenScanner = null;
       }
       if (tokenScanner == null)
       {
           
           while (lineScanner.hasNextLine())
           {
               String line = lineScanner.nextLine();
               lineNumber++;
               
               if (line.length() > 0 && line.split(" ").length > 0)
               {
                   tokenScanner = new Scanner(line);
                   break;
               }
           }
       }
       
       if (tokenScanner != null && tokenScanner.hasNextInt())
           anInt = tokenScanner.nextInt();
       else
           throw new IOException("Not integer (expect an integer value)");

       return anInt;
   } 

   private static String getTokenInLine(String pattern)
   {
       String aLine = null;
       
       if (tokenScanner != null && !tokenScanner.hasNext())
       {
           tokenScanner.close();
           tokenScanner = null;
       }
       if (tokenScanner == null)
       {
           while (lineScanner.hasNextLine())
           {
               String line = lineScanner.nextLine();
               lineNumber++;
               //ConfigGenerator.log("line " + lineNumber + ": " + line);
               if (line.length() > 0 && line.split(" ").length > 0)
               {
                   tokenScanner = new Scanner(line);
                   break;
               }
           }
       }
       
       if (tokenScanner.hasNext())
           aLine = tokenScanner.findInLine(pattern);

       return aLine;
   }   
   private static boolean hasToken()
   {
       boolean token_avail = false;
       
       if (tokenScanner != null)
       {
           token_avail = tokenScanner.hasNext();
       }
       if (!token_avail)
       {
           token_avail = lineScanner.hasNext();
       }

       return token_avail;
   }
   
   private static boolean hasToken(String pattern)
   {
       boolean token_avail = false;
       
       if (tokenScanner != null)
       {
           token_avail = tokenScanner.hasNext(pattern);
       }
       
       if (!token_avail)
       {
           token_avail = lineScanner.hasNext(pattern);
       }

       return token_avail;
   }

   private static boolean hasTokenInt()
   {
       boolean token_avail = false;
       
       if (tokenScanner != null)
       {
           token_avail = tokenScanner.hasNextInt();
       }
       
       if (!token_avail)
       {
           token_avail = lineScanner.hasNextInt();
       }

       return token_avail;
   }

// PRIVATE variables
   private static File configFile;
   private static boolean isReadToken;
   private static String token;
   private static int lineNumber;
   private static int groupLineNumber;
   private static int elementLineNumber;
   private static LinkedList<GroupStruct> groupConfig;
   private static LinkedList<NameStruct> errorConfig;
   
   private static Scanner lineScanner;
   private static Scanner tokenScanner;

   private static boolean floatingSupport;
   private static boolean enumSupport;

}
