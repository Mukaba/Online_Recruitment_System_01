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
 | Filename: welcome.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: welcome.inc
// show the welcome screen


    echo ("</td>");
    echo ("<td align='left' valign='top'>");
    echo ("<img src='".$GLOBALS['colorscheme']['topbar_right']."' alt='' />");
    echo ("</td>");
echo ("</tr>");
echo ("</table>");


echo ("<table border='0' cellspacing='0' cellpadding='20' width='100%'>");

echo ("<tr>");
    echo ("<td>");
    echo ("<br /><br />");
    echo ("<h2 align='center'>Co-op at ". ORGANIZATION_FULL ."</h2>");

    echo ("<h4 align='center' class='bodytext'>");

    if ($userlevel == ADMINSTUDENT)
    {
        $sql = "SELECT CONCAT(a.first_name, ' ', a.last_name) AS name 
            FROM student as a
            WHERE a.student_number='$student_number'";
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        echo $row["name"] . "<br />";
    }
    elseif ($userlevel == ADMINEMPLOYER)
    {
        $sql = "SELECT CONCAT(a.first_name, ' ', a.last_name) AS name 
            FROM contact as a
            WHERE a.contact_id='$employer_contact_id_login'";
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        echo $row["name"] . "<br />";
        $sql = "SELECT e.company_name
            FROM employer_company as e, employer_login as l
            WHERE e.employer_id=l.employer_id AND l.contact_id = '" . $employer_contact_id_login . "'";
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        echo $row["company_name"] . "<br />";
    }
    else
    {
        if ($userlevel == EMPLOYER){
            $sql = "SELECT e.company_name
                FROM employer_company as e, employer_login as l
                WHERE e.employer_id=l.employer_id AND l.contact_id = '" . $auth->contact_id . "'";
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            echo $row["company_name"] . "<br />";
        }
        echo($auth->name); 
        if (($userlevel == OFFICE) || ($userlevel == TRAFFICDIRECTOR)){
            $sql = "SELECT department_name FROM department
                WHERE department_id='$auth->department'";
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            echo("<br />".$row["department_name"]);
        }
    }

    echo ("</h4>");

    if (!((($userlevel == OFFICE) || ($userlevel == TRAFFICDIRECTOR))))
    {
        echo ("<br />");
        echo ("<p align='center'>Welcome to ".BRAND_NAME.", the information system for the Co-op programs at ". ORGANIZATION_FULL .".</p>");
        
        echo ("<h4 align='center'>Please select an option from the menu bar</h4>");
    }

    else
    {
        // this is where you can plug in content for the admin welcome page 
        $arr_returns = get_hooks('mamook.welcome_page.content', $select);

        if (sizeof($arr_returns)) {
            for($i = 0; $i < sizeof($arr_returns); $i++) {
                print $arr_returns[$i];
            }
        }
    }

    echo ("</td>");
echo ("</tr>");
    
echo ("</table>");
