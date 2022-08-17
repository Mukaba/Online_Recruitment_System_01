+------------------------------------------------------------------------------+
| Mamook(R) Software                                                           |
+------------------------------------------------------------------------------+
| Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
+------------------------------------------------------------------------------+
| THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
| ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
| PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
| PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
| RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
| RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
| ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
| FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
| COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
| ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
|                                                                              |
| Software distributed under the License is distributed on an "AS IS" basis,   |
| WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
| the specific language governing rights and limitations under the License.    | 
+------------------------------------------------------------------------------+
| Filename: README.txt                                                         |
+------------------------------------------------------------------------------+

Since this is an information system, the code cannot be used as is;  you will need 
to configure settings to match your server, set up an authentication system script, 
and populate various fields in the database before you will see any results.

Here is a list of steps to get you started:
- read the license file LICENSE.txt
- create a database (eg. in MySQL command line, "CREATE DATABASE MAMOOK")
- run SQL/mamook.sql on the database to populate the tables 
    (eg. from Linux command line, using MySQL, "mysql -u username -p MAMOOK < mamook.sql")
- the files in documentation/ contain a description of the tables and 
    fields in the various modules.  Using these files for assistance, 
    set up your departments, internal contacts, flags, etc. 
- copy the misc/release_constants_template file to misc/release_constants.inc.  
    Only modify misc/release_constants.inc, not the template.  Set all values in 
    misc/release_constants.inc
- copy the misc/secured_constants_template file to a secure area of your server that 
    is not web-accessible, and name the copy secured_constants.inc.  This file 
    contains passwords and other private information. Only modify 
    secured_constants.inc, not the template.  Set all values in 
    secured_constants.inc
- determine a method of authentication.  More information can be found in the file 
    documentation/logins.txt 

mamook.php is our main file -- all code (except for a few separate php files) goes 
through this main file.  mamook.php then uses includes to add in other code as 
necessary.  This is the place to start looking at the code.

You can set a bypass account name in misc/release_constants.inc so you can log in 
initially without setting up an authentication system.  Set ADMIN_ENABLED and/or 
STUDENT_ENABLED to TRUE, and set login names (ADMIN_LOGIN and STUDENT_LOGIN) in the 
secured_constants file. Using these logins, you can access the admin (OFFICE) 
userlevel and student userlevel without a password. In a production environment, 
always set ADMIN_ENABLED and STUDENT_ENABLED to FALSE.

Partway through our project, we switched to using the PEAR coding conventions for 
our PHP code.  As PEAR was in beta at the time, and has changed quite a bit since, 
our coding style has changed a few times to match its variations.  PEAR is now no longer 
in beta, so in future this should become less of a problem.  New code should be 
consistent with the latest PEAR standards, found at: 
http://pear.php.net/manual/en/standards.php

Mamook uses the ADOdb library as a database abstraction layer which supports a 
number of popular databases.  When coding, please use only wrapper calls; 
never use mysql (postgres, etc.) calls directly. See the SQL documentation on our 
website for more information.

If you need assistance with getting started, more documentation can be found on our 
website at: http://www.mamook.net.  Our Sourceforge page, accessible from our 
website, has forums that may also provide you with information and help.

Thanks for using Mamook(R) Software
