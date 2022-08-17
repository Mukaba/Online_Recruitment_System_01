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
 | Filename: traffic_flag_delete.inc                                            |
 +------------------------------------------------------------------------------+
 | This is the deletion confirmation page for when the user selects to delete   |
 | something from the edit/delete page                                          |
 +------------------------------------------------------------------------------+
*/
$continue = false;

$msg .= "<form method='post' name='myform2' action='".$PHP_SELF."&amp;select=flag_editor'>";
$msg .= "You have chosen to delete the following flags: <br /><br />"; 
$msg .= "<blockquote>";

//iterates through the list of flags to delete
foreach($remove AS $remove_flag)
{
    $msg .=  htmlspecialchars(stripslashes(getFlagName($remove_flag, strtolower($flag_type))),ENT_QUOTES)."<br />";

    //Retrieves the corresponding department codes of the shared departments to the flag
    $array = getFlagSharedDeptCode($flag_type, $department_id, $remove_flag, "dept_code");
    if(sizeof($array) > 0)
    {
        $msg .= "<blockquote>Delete all&nbsp;";
        $msg .= "<input type='checkbox' name='remove_all[".$remove_flag."]'>";
        $msg .= "&nbsp;".implode(", ",$array);
        $msg .= "</blockquote>";
    }
    $msg .= "<input type='hidden' name='delete_shared[]' value='".$remove_flag."'>";
    $msg .= "<br />";
}
$msg .= "</blockquote>";
$msg .= "<input type='hidden' name='flag_type' value='".$flag_type."'>";
$msg .= "<input type='hidden' name='department_id' value='".$department_id."'>";
$msg .= "<input type='submit' name='delete' value='Delete'>";
$msg .= "&nbsp;&nbsp;";
$msg .= "<input type='submit' name='continue' value='Cancel'>";
$msg .= "<input type='hidden' name='level1' value='". $level1 ."'>";
$msg .= "</form>";
instruct($msg);
?>
