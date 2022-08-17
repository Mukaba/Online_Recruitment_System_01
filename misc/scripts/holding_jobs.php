<?php

/*
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
   | Filename: holding_jobs.php                                                   |
   +------------------------------------------------------------------------------+                                                                    
   | Description: This script is invoked by a cron job that takes jobs out of     |
   |              24 hour hold. This script updates a contact's status once a     |
   |              job comes out of hold.                                          |
   +------------------------------------------------------------------------------+
 */

include('../release_constants.inc'); // release_constants to be included before constants.inc
include('../constants.inc');  // constants must be included before wwwclient.inc                                                                   
include('../names.inc');                                                                                                                           
include('../functions.inc');                                                                                                                       
include('../wwwclient.inc');                                                                                                                       
include(PATH_ADODB . 'adodb.inc.php');
include('../db.inc');                                                                                                                              
include('../resume_functions.inc');                                                                                                                
include('../special_flags_functions.inc');                                                                                                         

if ($job_id)
{
    $sql = ("
        SELECT contact_id
        FROM job_info
        WHERE job_id = '".$job_id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    $sql_dept = ("
        SELECT department_id
        FROM department_job_join
        WHERE job_id = '".$job_id."'
        ");
    $dept_result = $GLOBALS['dbh']->Execute($sql_dept);
    
    if ($dept_result->RecordCount())
    {
        while ($dept_row = $dept_result->FetchRow())
        {
            updateContactStatus($row['contact_id'],$dept_row['department_id'],$job_id,null);
        }
    }
}

?>
