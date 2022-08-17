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
 | Filename: add.inc                                                            |
 +------------------------------------------------------------------------------+
 | Description: This file is the start of adding contact info.  It decides      |
 | whether the user wants to add a company, division, or contact (with the      |
 | help of add_header.inc), and then loads up the correct page.                 |
 +------------------------------------------------------------------------------+

*/


include('contact/add/add_header.inc');
include('contact/add/AddContact.class');
include('contact/contact_functions.inc');

switch($level1)
{
    case "department":
        if ($userlevel == OFFICE || $userlevel == ADMINISTRATOR || $userlevel == TRAFFICDIRECTOR)
        {
            include('contact/add/department/add_department_header.inc');
        }
        else
        {
            include('misc/loguser.inc');
        }
    break;
    case "contact":
        if ($userlevel == OFFICE || $userlevel == ADMINISTRATOR || $userlevel == TRAFFICDIRECTOR)
        {
            include('contact/add/contact/add_contact_header.inc');
        }
        else
        {
            include('misc/loguser.inc');
        }
    break;
    case "company":
        if ($userlevel == OFFICE || $userlevel == ADMINISTRATOR || $userlevel == TRAFFICDIRECTOR)
        {
            include('contact/add/company/add_company_header.inc');
        }
        else
        {
            include('misc/loguser.inc');
        }
        break;
    default:                                                                        //let's look up the default from preferences
        $sql = "SELECT b.employer_info_menu
                FROM contact AS a, contact_internal AS b
                WHERE b.contact_id = a.contact_id
                AND b.department_id = '". $auth->department ."' ";

        if ($_SESSION['SESSION_netlog']==SCRIPT_LOGIN)                                           //Check which type of login the user is using
        {
            $sql = $sql . "AND b.netlink_id='$login_id'";                           //they are using netlink
        }
        else
        {
            $sql = $sql . "AND b.login_id='$login_id'";                             //they are using NIS
        }

        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $employer_info_menu = $row['employer_info_menu'];

        if ($userlevel == OFFICE || $userlevel == ADMINISTRATOR || $userlevel == TRAFFICDIRECTOR)
        {
            if($employer_info_menu == 0)                                            //set in preferences -> general
            {
                include('contact/add/company/add_company_header.inc');
            }
            elseif($employer_info_menu == 1)
            {
                include('contact/add/contact/add_contact_header.inc');
            }
        }
        else
        {
            include('misc/loguser.inc');
        }
        break;
} //switch

?>
