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
   | Filename: check_contact_status.inc                                           |
   +------------------------------------------------------------------------------+                                                                    
   | Description: This script is used to check contact statuses very quickly.     |
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

    $color[0] = "#FFFFFF";
    $color[1] = "#A9ACFF";

    if ($_SERVER['SERVER_ADDR'] != $_SERVER['REMOTE_ADDR'] && !in_array($_SERVER['REMOTE_ADDR'],$IPS_FOR_DEBUG))
    {
            exit("Security Violation");
    }
?>
<html>
<body>
<form action="check_contact_status.php" method="post">
Contact ID: <input type="text" name="contact_id" value="<?php echo($contact_id) ?>" style="background:#A9ACFF"><input type="submit" value="submit">
</form>
<?php
    // Grab contact information including the division and company it belongs to. 
    if ($contact_id)
    {
        $sql = ("
            SELECT c.first_name, c.last_name, ce.department_id, ce.employer_id
            FROM contact c
            INNER JOIN contact_employer ce
            ON c.contact_id = ce.contact_id
            WHERE ce.contact_id = '".addslashes($contact_id)."'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        $contact_name = $row['first_name']." ".$row['last_name'];
        $division_id = $row['department_id'];
        $employer_id = $row['employer_id'];

        $sql = ("   
            SELECT department_name 
            FROM employer_department
            WHERE department_id = '".$division_id."'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $division_name = $row['department_name'];

        $sql = ("
            SELECT company_name
            FROM employer_company
            WHERE employer_id = '".$employer_id."'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $company_name = $row['company_name'];

        echo("Contact: ".$contact_name." (".$contact_id.")<br />");
        echo("Division: ".$division_name." (".$division_id.")<br />");
        echo("Company: ".$company_name." (".$employer_id.")<br />");

        // output contact's status to a table. 
        echo("<table border='1'>");
        $sql = ("
            SELECT dcs.contact_id, dcs.department_id, eis.status_name, dcs.entered_on, dcs.activity_date, dcs.history_id, dcs.job_id
            , dcs.term_id, dcs.year, dcs.current_status, eiat.activity_type_name 
            FROM department_contact_status dcs
            INNER JOIN employer_info_statuses eis 
            ON dcs.status_id = eis.status_id
            INNER JOIN employer_info_activity_type eiat
            ON dcs.activity_type_id = eiat.activity_type_id
            WHERE dcs.contact_id = '".addslashes($contact_id)."'
            ORDER BY dcs.department_id, dcs.current_status, dcs.entered_on
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        $i = 0;
        $j = 0;

        while($row = $result->FetchRow())
        {
            if ($i==0)
            {
                $temp_dept = $row['department_id'];
                echo("<tr bgcolor='".$color[$j++]."'>");
                foreach($row as $key => $value)
                {
                    if ($i == 0)
                    {
                        echo("<td>".$key."</td>");
                    }
                }
                echo("</tr>");

            }
            else
            {
                if ($temp_dept != $row['department_id'])
                {
                    $j = ++$j % 2;
                    $temp_dept = $row['department_id'];
                }
            }
            echo("<tr bgcolor='".$color[$j]."'>");
            foreach($row as $key => $value)
            {
                echo("<td>".$value."</td>");
                    
            }
            echo("</tr>");
            $i++;

        }
        echo("</table>");

        echo("<br />");

        // output contact's division status
        echo("<table border='1'>");

        $sql = ("
            SELECT dds.division_id, dds.department_id, eis.status_name, dds.entered_on, dds.current_status
            FROM department_division_status dds
            INNER JOIN employer_info_statuses eis 
            ON dds.status_id = eis.status_id
            WHERE dds.division_id = '". $division_id ."'
            ORDER BY dds.department_id, dds.current_status, dds.entered_on
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        $i = 0;
        $j = 0; 
        while($row = $result->FetchRow())
        {
            if ($i==0)
            {
                $temp_dept = $row['department_id'];
                echo("<tr bgcolor='".$color[$j++]."'>");
                foreach($row as $key => $value)
                {
                    if ($i == 0)
                    {
                        echo("<td>".$key."</td>");
                    }
                }
                echo("</tr>");

            }
            else
            {
                if ($temp_dept != $row['department_id'])
                {
                    $j = ++$j % 2;
                    $temp_dept = $row['department_id'];
                }
            }
            echo("<tr bgcolor='".$color[$j]."'>");
            foreach($row as $key => $value)
            {
                echo("<td>".$value."</td>");
            }
            echo("</tr>");
            $i++;

        }
        echo("</table>");

        echo("<br />");

        // output contact's company status
        echo("<table border='1'>");

        $sql = ("
            SELECT dcs.employer_id, dcs.department_id, eis.status_name, dcs.entered_on, dcs.current_status
            FROM department_company_status dcs
            INNER JOIN employer_info_statuses eis 
            ON dcs.status_id = eis.status_id
            WHERE dcs.employer_id = '". $employer_id."'
            ORDER BY dcs.department_id, dcs.current_status, dcs.entered_on
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $i = 0;
        while($row = $result->FetchRow())
        {
            if ($i==0)
            {
                $temp_dept = $row['department_id'];
                echo("<tr bgcolor='".$color[$j++]."'>");
                foreach($row as $key => $value)
                {
                    if ($i == 0)
                    {
                        echo("<td>".$key."</td>");
                    }
                }
                echo("</tr>");

            }
            else
            {
                if ($temp_dept != $row['department_id'])
                {
                    $j = ++$j % 2;
                    $temp_dept = $row['department_id'];
                }
            }
            echo("<tr bgcolor='".$color[$j]."'>");
            foreach($row as $key => $value)
            {
                echo("<td>".$value."</td>");
            }
            echo("</tr>");
            $i++;

        }
        echo("</table>");
    }
?>

</body>
</html>
