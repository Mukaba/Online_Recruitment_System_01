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
   | Filename: display_results.inc                                                |
   +------------------------------------------------------------------------------+
   | Description:                                                                 |
   +------------------------------------------------------------------------------+

*/


/*
   This file takes what's given to it from the other form and displays the results, ONLY if the contact exists
   in the login table.

   first_name(_letter)
   last_name(_letter)
   company_name


*/

$sql = "select concat(c.first_name, ' ', c.last_name) as name, ec.company_name, c.contact_id, el.login_id
    from contact as c, employer_company as ec, contact_employer as ce
    LEFT join employer_login as el on (c.contact_id = el.contact_id)
    where c.contact_id = ce.contact_id AND ec.employer_id = ce.employer_id AND 
    c.first_name like '" . (($first_name) ? $first_name : $first_name_letter) . "%' AND 
    c.last_name like '". (($last_name) ? $last_name : $last_name_letter) . "%' ";

if ($company_name)
{
    $sql .= "AND ec.company_name like '$company_name%' AND ec.employer_id = ce.employer_id ";
}

$sql .= "Order by c.last_name,c.first_name";


$result = $GLOBALS['dbh']->Execute ($sql);

// check for results first


if ($result->RecordCount()) 
{

?>
    <h4>Choose a Contact to Log In as:</h4>
    <form method='post' action="<?php echo($PHP_SELF); ?>">
    <table border="1" cellspacing=0 cellpadding=0>
    <tr>
    <td>

    <table border="0" cellpadding=2 width="100%">

    <tr>
    <td class="rowgrey" align='center'>&nbsp; &nbsp; </td>
    <td class="rowgrey" align='center'><b class='white'>&nbsp; Contact Name &nbsp;</b></td>
    <td class="rowgrey" align='center'><b class='white'>&nbsp; Company &nbsp;</b></td>
    <td class="rowgrey" align='center'><b class='white'>&nbsp; Has Login &nbsp;</b></td>
    </tr>

<?php

    $i = 0;
    $flag = 0;
    while ($row = $result->FetchRow()) 
    {
        $rowclass = (($i % 2 == 0) ? "row1d" : "row0d");
        $i ++;

        echo ("<tr>");
        echo ("<td class='$rowclass' align='center'> ");
        if(($row["login_id"]))
        {
            echo("<input type='radio' name='employer_contact_id_login' value='" . $row["contact_id"] . "' class='$rowclass' onclick='submit()' />" );
            $flag = 1;
        } else {
            echo("&nbsp;");
        }
        echo("</td>");
        echo ("<td class='$rowclass' align='left'> &nbsp;" . $row["name"] . "&nbsp; </td>");
        echo ("<td class='$rowclass' align='center'> &nbsp;" . $row["company_name"] . "&nbsp; </td>");
        echo ("<td class='$rowclass' align='center'>" . (($row["login_id"]) ? "Yes" : "No") . " </td>");
        echo ("</tr>");

    }

?>
    </table>
    </td>
    </tr>
    </table>

    <br />
<?php
    if($flag == 1)
    {
        echo("<input type='hidden' name='select' value='employer_login' />");
        echo("<input type='hidden' name='admin_employer_login' value='yes' />");
        echo("<input type='submit' value='Log In as Contact' />");
        echo("</form>");
    } else {
        notify("The chosen contacts do not have login IDs");
    }

} else {

    notify ("No contacts found based on search criteria");
    include ('misc/employer_login/enter_info.inc');
}

?>
