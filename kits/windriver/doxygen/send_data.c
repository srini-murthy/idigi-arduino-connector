/*! @page send_data_sample Send Data Sample
 *
 * @htmlinclude nav.html
 *
 * @section send_data_overview Overview
 *
 * This sample demonstrates how to send data to the iDigi Server
 * using the idigi_send_data() API. The data is stored as a file on the iDigi Server.
 *
 * @section device_request_description Code Flow
 *
 * The routine main() calls the API idigi_send_data(), this performs the file transfer to the
 * iDigi Device Cloud.  The path field contains the file name and the data structure idigi_connector_data_t contains information about the file
 * which is transferred.  The routine idigi_send_data() is called in a loop which iterates every
 * 60 seconds, each time overwriting the existing file on the iDigi Device Cloud.
 * 
 * @section send_data_build Building
 *
 *    1. Click on Wind River Workbench icon, this is used to build, debug and launch the idigi sample
 *       applications.
 * 
 *    2. The sample applications appear on the left hand side of the workbench.
 * 
 *    3. Click on idigi_send_data
 * 
 *    4. Right mouse click and select Build Project.
 *
 * In the Build Console on the lower portion of the screen you can view the build output.
 *
 *
 * @subsection send_data_source_files Source Files
 *
 * The following is a list of source files required to build this sample:
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 * <th class="title">Name</th>
 * <th class="title">Description</th>
 * <th class="title">Location</th>
 * </tr>
 * <tr>
 * <th>main.c</th>
 * <td>Entry point of program, calls @endhtmlonly idigi_send_data() @htmlonly in a loop</td>
 * <td>workspace/idigi_send</td>
 * </tr>
 * <tr>
 * <th>idigi_connector.h</th>
 * <td> iDigi Connector API</td>
 * <td>usr/lib</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection include_paths Include Paths
 *
 * The following include paths are required:
 *
 * @li usr/include
 * 
 * @section web_gui Viewing results from the iDigi Device Cloud
 * 
 * 
 * The data transferred is stored in the file test.txt in the folder for the device.  This file can
 * be viewed from iDigi Manager Pro or retrieved using the python script get file which is described
 * below.
 * 
 * To view the results for this sample login to the iDigi Device Cloud.
 * Once logged in click on the Data Services tab as shown below. Double click to the device folder.
 * 
 * @image html send_data_sample1.png
 * 
 * This sample creates a file test.txt in the test directory.  Double click on the test folder to
 * open the test directory.
 *
 * @image html send_data_sample2.png
 *
 * The file is then listed as shown (below). You can click on a 
 * file to view its contents, you will see the file test.txt in the test directory
 * after you run this sample.
 *
 * @image html send_data_sample3.png
 *
 * @section python_script Python script to retrieve file from the iDigi Device Cloud.
 *
 * This sample provides a simple python script to retrieve the file test.txt from developer.idigi.com
 * using @htmlonly <a href="web_services.html">iDigi Web Services.</a> @endhtmlonly
 * To run the python script for this sample:
 *
 * @code
 * python get_file.py <username> <password> <device_id>
 * @endcode
 *
 * @note The device_id must be in the long format, for example: python get_file.py username password 00000000-00000000-00b338ff-ff0002d8 where the MAC address of the device is 00:b3:38:00:02:d8
 *
 *
 * @htmlinclude terminate.html
 */
