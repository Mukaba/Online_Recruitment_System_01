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
 | Filename: functions.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description: This file has functions that are used by other inc files        |
 +------------------------------------------------------------------------------+

*/

function getFolderName($folder_id, $student_number)
{
    $sql=("
        SELECT folder_name
        FROM student_job_folder_name
        WHERE student_number = '".$student_number."'
        AND folder_id = '".$folder_id."'");
    $result = $GLOBALS['dbh']->Execute($sql);
    if($result->RecordCount() > 0)
    {
        while ($row = $result->FetchRow())
        {
            $folder_name = $row["folder_name"];
        }
    }
    else
    {
        $fsql = ("
                SELECT folder_id, default_name
                FROM student_job_folder
                WHERE folder_id = '".$folder_id."'
                ");
        $fresult = $GLOBALS['dbh']->Execute($fsql);
        $frow = $fresult->FetchRow();
        $folder_name = $frow["default_name"];
    }
    return $folder_name;
}

function getFolderSize($folder_id, $student_number, $status)
{
    $sql=("
            SELECT sjfj.job_id
            FROM job_info as a
            INNER JOIN student_job_folder_join AS sjfj ON sjfj.job_id=a.job_id AND sjfj.student_number = '".$student_number."' 
            AND sjfj.folder_id='".$folder_id."' 
            ");

    if($status == "closed")
    {
        $sql.= "WHERE (a.status=" . CLOSED . " OR (a.status=" . CANCELLED . " AND a.student_status='" . STUDENT_NOT_HIRING . "'))";
    }
    elseif($status == "open")
    {
        $sql.= "WHERE a.status=" . POSTED;
    }
    
    $result = $GLOBALS['dbh']->Execute($sql);
    return $result->RecordCount();
}


?>
