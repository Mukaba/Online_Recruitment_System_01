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
 | Filename: company_list.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file displays all of the companies to the user, allowing   |
 | them to choose one (for which they will add a division to), or to create a   |
 | new one.                                                                     |
 +------------------------------------------------------------------------------+

*/


?>

<script type="text/javascript" language="javascript">
<!--javascript

function renew()
{
	document.form1.submit();
}

//-->

</script>

<?php

/*
 First off, we need to find which letters to allow the user to click on across the
 screen.  If there's no companies in the database for the letter Q, there's no 
 point in showing that as an option to click on.
*/

$sql = ("
	SELECT DISTINCT SUBSTRING(company_name, 1, 1) AS order_letter
	FROM employer_company
	WHERE company_name IS NOT NULL
	AND company_name!=''
	AND company_display
	ORDER BY company_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
	$order_choices[] = strtoupper($row["order_letter"]);
}

if (!$order)
{
	$order = $order_choices[0];
}

$order_href = $PHP_SELF . "&amp;select=add_contact&amp;level1=department";

$sql = ("
	SELECT DISTINCT employer_id, company_name
	FROM employer_company
	WHERE company_name IS NOT NULL
	AND company_name!=''
	AND company_name LIKE '" . $order . "%'
	AND company_display
	ORDER BY company_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

if ($order_choices)
{
	
	echo("<center>");
	
	echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");
	
	echo("<tr>");
		echo("<td colspan='2'>");
	        echo("<br />");
		echo("</td>");
	echo("</tr>");
	
	echo("<tr>");
		echo("<td colspan='2'>");
		echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
		echo("<tr>");
			echo("<td colspan='2' class='row0'>");
            echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
			echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
	echo("<tr>");
		echo("<td align='center' colspan='2' nowrap='nowrap'><b class='black'>Choose a company to add the new division to:</b></td>");
	echo("</tr>");
			echo("<tr>");
				echo("<td colspan='2' align='center' nowrap='nowrap'>");
				for ($i = 0; $i < (sizeof($order_choices) - 1); $i++)
				{
					echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a> | ");
				}
				echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a>");
	                        echo("</td>");
	                echo("</tr>");
	                echo("<tr>");
				echo("<td colspan='2'  align='center'>");
	                        $size = 10;
	                        if ($result->RecordCount() < 10)
	                        {
	                                $size = $result->RecordCount() + 1;
	                        }
				if ($result->RecordCount() > 0)
				{
	                        	echo("<select name='employer_id' onchange='renew()' size='" . $size . "'>");
	                        	echo("<option value='' ");
	                        	if (!$employer_id || $employer_id == '')
	                        	{
	                        	        echo("selected='selected'");
	                        	}
	                        	echo(">&nbsp;</option>");
	                        	while ($row = $result->FetchRow())
	                        	{
	                        	        echo("<option value='" . $row["employer_id"] . "' ");
	                        	        if ($employer_id == $row["employer_id"])
	                        	        {
	                        	                echo("selected='selected'");
	                        	        }
	                        	        echo(">" . $row["company_name"] . "</option>");
	                        	}
	                        	echo("</select>");
				}
				else
				{
					echo("<br />No companies starting with '" . $order . "'.<br /><br />");
				}
                echo("<input type='hidden' name='select' value='add_contact' />");
                echo("<input type='hidden' name='level1' value='department' />");
                echo("<input type='hidden' name='order' value='" . $order . "' />");
                echo("</td>");
                echo("</tr>");
            echo("</table>");
            echo("</form>");
                echo("</td>");
                echo("</tr>");
            echo("</table>");
            echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
                echo("<tr>");
                    echo("<td colspan='2' align='center'>-Or-<br /><br />");
                    echo("</td>");
                echo("</tr>");
                echo("<tr>");
                    echo("<td colspan='2' align='center'>");
                        echo("<form method='post' action='" . $PHP_SELF . "'>");
                        echo("<input type='hidden' name='select' value='add_contact' />");
                        echo("<input type='hidden' name='level1' value='company' />");
                        echo("<input type='submit' name='new_company' value='Add a New Company' />");
                        echo("</form>");
                    echo("</td>");
                echo("</tr>");
            echo("</table>");
            echo("</td>");
        echo("</tr>");
	echo("</table>");
	echo("</center>");
	
}
else
{
	/*
	 There are no companies in the database.  Let the user know this, and give them the option of adding a new company.
	*/
	
	notify("There are currently no companies in the database.  You must add a company before you can add a division.");
}

?>
