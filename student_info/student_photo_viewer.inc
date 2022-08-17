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
 | Filename: student_photo_viewer.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file retrieves images from a folder that is restricted     |
 | to the outside world, and sends the image to the browser if the user has     |
 | the proper user level.                                                       | 
 +------------------------------------------------------------------------------+

*/
$sql = ("
    SELECT s.photo
    FROM student s 
    INNER JOIN student_department sd
    ON s.student_number = sd.student_number
    WHERE sd.record_id = '".$record_id."'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

// Only allow people who've logged in to run this script. Or, allow any local server processes run this script. 
if ((
    ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR || $userlevel == FACULTY) 
    || $_SERVER['SERVER_ADDR'] == $_SERVER['REMOTE_ADDR'] 
    || (is_array($IPS_FOR_DEBUG) && sizeof($IPS_FOR_DEBUG) && in_array($_SERVER['REMOTE_ADDR'],$IPS_FOR_DEBUG))
    ) 
    && $record_id && $result->RecordCount())
{
    header( "Content-type: image/jpg" );
    @readfile(PHOTO_DIR.$row['photo']);
}
// Otherwise, unauthorized use, bounce user back
else
{
    header( "Location: ../".$PHP_SELF);
}
ob_end_flush();
?>
