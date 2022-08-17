#!/usr/local/bin/php
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
 | Filename: scan_duplicate_netlink_id.php                                      |
 +------------------------------------------------------------------------------+
 | Description: This script is used to detect duplicate netlink ids in the      |
 |              student table and report it to an administrator. Duplicate      |
 |              netlink ids can cause problems for students logging in.         |
 +------------------------------------------------------------------------------+

 Note: This script is run as a command line cron job. 
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
include('../email.inc');

if ($_SERVER['SERVER_ADDR'] != $_SERVER['REMOTE_ADDR'] && !in_array($_SERVER['REMOTE_ADDR'],$IPS_FOR_DEBUG))
{
    exit("Security Violation");
}
else
{
    // grab all the netlink ids in the database 
    $sql = ("
        SELECT DISTINCT netlink_id
        FROM student
        WHERE netlink_id IS NOT NULL and netlink_id != '' and netlink_id != '*'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);

    $arr_problem_student_number = array(); 

    while($row = $result->FetchRow()) {
        $netlink_id = $row['netlink_id'];

        $sql2 = ("
            SELECT student_number
            FROM student 
            WHERE netlink_id = '".$netlink_id."'
            ");
        $result2 = $GLOBALS['dbh']->Execute($sql2);

        // check for duplicate netlink ids
        if ($result2->RecordCount() > 1) {
            $temp_student_numbers = null; 
            while ($row2 = $result2->FetchRow()) {
                $temp_student_numbers .= $row2['student_number']." ";

            }
            $arr_problem_student_number[] = $temp_student_numbers."\n";
        }
    }

    // report duplicates to an admin 
    if (sizeof($arr_problem_student_number))
    {
        $to = DB_EMAIL_TO;
        $from = BRAND_NAME_SYSTEM_EMAIL;
        $subject = "Mamook - Netlink ID Collision (".DATABASE.")";
        $body = date("F j, Y (g:i a)")."\n\n";
        $body .= "Student Numbers With Duplicate Netlink IDs:\n\n";
        for ($i = 0; $i < sizeof($arr_problem_student_number); $i++)
        {
            $body .= $arr_problem_student_number[$i]."\n";
        }

        $email = new email($to, $subject, $body);
        $email->from = $from;
        $email->send();
    }
}

?>
