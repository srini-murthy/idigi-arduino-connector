 /*! @mainpage iDigi Integration Kit
 *
 * @htmlinclude nav.html
 *
 * @section table_of_contents Overview
 *
 *          -# @ref intro
 *              -# @ref features
 *              -# @ref communicating
 *          -# @ref requirements
 *          -# @ref language
 *          -# @ref features
 *          -# @ref threading
 *          -# @ref code_organization
 *              -# @ref private
 *              -# @ref public
 *              -# @ref run
 *              -# @ref platforms
 *              -# @ref samples
 *          -# @ref zlib
 *
 * @section intro Introduction
 *
 * The iDigi Integration Kit (IIK) is a software development package used to
 * communicate and exchange information between a device and the iDigi Device
 * Cloud. iDigi supports application to device data interaction (messaging),
 * application & device data storage, and remote management of devices. Devices
 * are associated with the iDigi Device Cloud through the Internet or other wide
 * area network connection, which allows for communication between the device, the
 * iDigi Device Cloud, and customer applications. An important part of this communication
 * is the transfer of data from a device to the iDigi Device Cloud.
 * iDigi is based upon a cloud computing model that provides on-demand scalability so
 * you can rest assured that when you need additional computing and storage iDigi will
 * scale to meet your needs. The iDigi Device Cloud is designed using a high-availability
 * architecture, with redundancy and failover characteristics in mind.
 * Using iDigi, customers can now easily develop cloud connected devices and
 * applications that quickly scale from dozens to hundreds, thousands or even
 * millions of endpoints.
 *
 * @image html cloud.jpg 
 *
 * @section language Language Support
 *
 * The software provided is C89, C99 and ANSI compliant. The sample platforms provided use standard C 
 * calls which are available in most operating systems; the networking portion of the 
 * sample uses Berkley sockets calls.  If you are running on a Linux i486 based platform 
 * and using the GNU toolchain the samples can be run without any modifications.
 *
 * @section requirements Platform Requirements
 *
 * @htmlonly
 * <table border="8">
 * <tr>
 * <th>Description</td>
 * <th>Size With Compression On</td>
 * <th>Size With Compression Off</td>
 * <th>Notes</td>
 * </tr>
 * <tr>
 * <td>Heap</td>
 * <td>9232 bytes</td>
 * <td>2704 bytes</td>
 * <td>IIK memory usage</td>
 * </tr>
 * <tr>
 * <td>Stack</td>
 * <td>TBD bytes</td>
 * <td>TBD bytes</td>
 * <td>Maximum Stack Usage</td>
 * </tr>
 * <tr>
 * <td>Code</td>
 * <td>17393 bytes</td>
 * <td>17393 bytes</td>
 * <td>On a 32-bit i486 Linux machine, using GCC v4.4.4, optimized for size (-Os)</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *  
 * @subsection features IIK Features
 *     @li Send data to and from a device over the iDigi Device Cloud
 *     @li Update firmware on the device
 *     @li View the device configuration
 *     @li Reboot a device
 *
 * @subsection communicating Communicating with your device
 *
 * To manage your device you can use the iDigi Device Cloud Portal, this contains
 * the graphical user interface to the iDigi Device Cloud, this tool allows for
 * complete management of your device.
 *
 * To communicate with your device remotely without using the graphical interface
 * to the iDigi Device Cloud you can use @b iDigi @b Web @b Services.  Web service 
 * requests are sent from an application to the iDigi Clould which then communicates 
 * with the device, this allows for bidirectional machine to machine communication.
 * Python samples are provided to show how to communicate to a device over the
 * iDigi Device Cloud.
 *
 * @section threading Threading Model
 *
 * The IIK can be run in a multithreaded or single threaded environment, it is
 * suggested in a multithreaded OS that the IIK be run as a separate thread, in a
 * single threaded system a step routine is provided which runs a portion of the
 * IIK and must be called periodically.  In the multithreaded model the IIK provides
 * a routine which runs the IIK and does not return, this routine is intended
 * to be run as separate thread.  The IIK is designed to run in a preemptive or
 * cooperative multithreaded system, the run routine will call sleep to relinquish
 * control of the CPU.
 *
 * @note You must decide before proceeding if you want to run as a single threaded
 * model (step) or run the IIK as a separate thread (run).  In a small OS with no
 * RTOS you should use the step routine, otherwise you should use the run routine.
 *
 * @section code_organization Source Code Organization
 *
 * When uncompressed the directory structure below will be created in the idigi directory.
 * The public directory is divided into step and run, use the appropriate directory
 * based on your threading model.
 *
 * @htmlonly
 * <table border="8">
 * <tr>
 * <th>Directory</td>
 * <th>Description</td>
 * </tr>
 * <tr>
 * <td>private</td>
 * <td>IIK Library Code  <strong>(do not modify)</strong></td>
 * </tr>
 * <tr>
 * <td>public/include</td>
 * <td>Public API</td>
 * </tr>
 * <tr>
 * <td>public/run</td>
 * <td>Examples of running the IIK as a separate thread</td>
 * </tr>
 * <tr>
 * <td>public/step</td>
 * <td>Examples of running the IIK in a single threaded model</td>
 * </tr>
 * <tr>
 * <td>public/run/platforms/linux</td>
 * <td>Code used to interface to a linux system</td>
 * </tr>
 * <tr>
 * <td>public/run/platforms/template</td>
 * <td>Shell routines to implement a new platform from scratch</td>
 * </tr>
 * <tr>
 * <td>public/run/samples</td>
 * <td>Samples for the IIK</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection private private Directory
 * The private directory contains all the files which are used to build the IIK library. 
 * The user @b should @b not @b modify @b any @b files @b in @b this @b directory. These files are only provided 
 * so the library can be built using the tool chain for your platform.
 *
 * @subsection public include Directory
 * The public/include directory contains header files used globally by the IIK. 
 * When porting to a new platform the user may need to modify the file idigi_types.h. 
 * The file idigi_api.h in this directory is the IIK public API.
 *
 * @subsection run run Directory
 * Directory containing platforms and samples which run the IIK as a separate 
 * thread in a multitasking environment.
 *
* @subsection step step Directory
 * Directory containing platforms and samples which run the IIK as a single thread
 * in a non-multhreaded model.
 *
 * @subsection platforms platforms Directory
 * For each supported platform there is a subdirectory along with a set of interface routines.
 * The Getting Started Procedure will walk you through setting up your platform.
 *
 * @subsection samples samples Directory
 * Samples on how to use the IIK, the compile_and_link sample is used to verify
 * that your new envirorment is able to build. There is a sample for each major
 * feature in the IIK, there is documentation in this guide for each sample.
 *
 * @section zlib zlib Support
 * The zlib software library is used for data compression by the IIK, the zlib library 
 * is required if compression is to be supported by your device. Data compression is 
 * used to reduce the network traffic load. If your application does not require compression, 
 * ignore this section. The zlib source code is not under the GNU license, the license is described in zlib.h.
 * The zlib home page is located at: http://zlib.net/. If your platform does not already have the zlib library 
 * you will need to download and build the zlib library. The header file zlib.h is included by the IIK and 
 * must be in the include path for the IIK library.  Instructions on how to build zlib
 * are provided with the package.
 *
 * @note The first step is to read the @ref getting_started page.
 *
 * </td></tr>
 * </table>
 * @endhtmlonly
 */

/*! @page troubleshooting Troubleshooting
  Read this page if you are having trouble with your device.
  @section sec An example section
  This page contains the subsections @ref subsection1 and @ref subsection2.
  For more info see page @ref page2.
  @subsection subsection1 The first subsection
  Text.
  @subsection subsection2 The second subsection
  More text.
*/

/*! @page sample_code Examples
  Read this page if you are having trouble with your device.
  @section sec An example section
  This page contains the subsections @ref subsection1 and @ref subsection2.
  For more info see page @ref page2.
  @subsection subsection1 The first subsection
  Text.
  @subsection subsection2 The second subsection
  More text.
*/
/** 
* \def KIT_NAME 
* IIK 
*/ 


