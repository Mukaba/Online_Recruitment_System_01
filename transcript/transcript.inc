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
 | Filename: transcript.inc                                                     |
 +------------------------------------------------------------------------------+
 | Description: This file determines which file(s) to include based on the      |
 | $page variable and the userlevel.                                            |
 +------------------------------------------------------------------------------+

*/


if ($userlevel == OFFICE || $userlevel == FACULTY)
{
    if ($department_id == "")
    {
        if ($departments_in_group > 1)
        {
            if ($_SESSION['SESSION_netlog']==SCRIPT_LOGIN) 
            {
                $group_sql = "SELECT b.pulldown_menu_group
                    FROM contact_internal AS b
                    WHERE b.department_id='$auth->department' AND b.netlink_id='$login_id'";
            } else {
                $group_sql = "SELECT b.pulldown_menu_group
                    FROM contact_internal AS b
                    WHERE b.department_id='$auth->department' AND b.login_id='$login_id'";
            }
            $result_group=$GLOBALS['dbh']->Execute($group_sql);
            while($row=$result_group->FetchRow())
            {
                $pulldown_menu_group=$row['pulldown_menu_group'];
            }
            if ($pulldown_menu_group=='1')
            {
                $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
                $department_id='0';
            } else {
                $department_id = $auth->department;
            }
        } else {
            $department_id = $auth->department;
        }
    }


    if ($department_id != "0" && !in_array($department_id, $departments_in_group))
    {
        $error="You do not have access to this department.";
        unset($PDF);
        $page = "";
        $department_id = $auth->department;
    }
    if ($department_id == "0")
    {
        $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
    } else {
        $department_in_str = "'" . $department_id . "'";
    }
}

if ($PDF) 
{
    switch ($page)
    {
        case "View PDF":
            if ($userlevel == STUDENT || $userlevel == ADMINSTUDENT)
            {
                include( "transcript/student/view.inc" );
            } elseif ($userlevel == OFFICE || $userlevel == FACULTY) {
                include( "transcript/admin/view.inc" );
            } else {
                unset( $PDF );
            }
        break;
        case "View HTML":
            if ($userlevel == STUDENT || $userlevel == ADMINSTUDENT)
            {
                include( "transcript/student/view.inc" );
            } elseif ( $userlevel == OFFICE || $userlevel == FACULTY) {
                include( "transcript/admin/view.inc" );
            } else {
                unset( $PDF );
            }
        break;
        case "Save":
            if ($userlevel == OFFICE)
            {
                include( "transcript/save.inc" );
                include( "transcript/admin/view.inc" );
            } else {
                unset( $PDF );
            }
        break;
        default:
        $header = 1;
        unset ($PDF);
    }
} elseif (!$PDF) {
    if ($userlevel == STUDENT || $userlevel == ADMINSTUDENT) {
        include ("transcript/transcript_header.inc");
        include ("transcript/student/view.inc");
        include ("transcript/transcript_footer.inc");
    } 
    elseif ($userlevel == OFFICE || $userlevel == FACULTY) {
        if ($new_page=="choose_student")
        {
            include ("transcript/admin/header.inc");
            include ("transcript/admin/choose_student.inc");
            include ("transcript/admin/footer.inc");
        } else {
            include ("transcript/admin/header.inc");
            if ($error != "")
            {
                error($error);
            }
            include ("transcript/admin/view_main.inc");
            include ("transcript/admin/footer.inc");
        }
    }else{
        include("welcome.inc");
    }
}
?>
