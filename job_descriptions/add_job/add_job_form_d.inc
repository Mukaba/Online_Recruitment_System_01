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
 | Filename: add_job_form_d.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file is used when the user wishes to add a new company,    |
 | division, or contact.                                                        |
 +------------------------------------------------------------------------------+

*/

echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");

echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");

echo("<tr>");
	echo("<td colspan='2'>");
    echo("<input type='hidden' name='select' value='add_job' />");
    echo("<input type='hidden' name='add_this_object' value='" . $add_this_object . "' />");
        echo("<br />");
	echo("</td>");
echo("</tr>");

if ($add_this_object == 'employer')
{
	echo("<tr>");
		echo("<td colspan='2'>");
		echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
		echo("<tr>");
			echo("<td align='center' colspan='2' class='row0'>");
			echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
			echo("<tr>");
				echo("<td align='center' colspan='2' nowrap='nowrap'><b class=black>Add a New Company:</b></td>");
			echo("</tr>");
	                echo("<tr>");
				echo("<td colspan='2'>&nbsp;</td>");
			echo("</tr>");
			echo("<tr>");
				echo("<td align='right'>Company Name:</td>");
				echo("<td align='left'><input type='text' name='company_name' size='50' maxlength='75' value='" . $company_name . "' /></td>");
			echo("</tr>");
			echo("<tr>");
				echo("<td align='right'>Company Website:</td>");
				echo("<td align='left'><input type='text' name='company_website' size='45' maxlength='60' value='" . $company_website . "' /></td>");
			echo("</tr>");
			echo("<tr>");
				echo("<td align='right'>Company Description:</td>");
				echo("<td align='left'><textarea name='company_description' rows='10' cols='40'>" . $company_description . "</textarea></td>");
			echo("</tr>");
			echo("</table>");
			echo("</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td colspan='2' align='center'><input type='submit' value='Continue' /></td>");
			echo("<input type='hidden' name='continue' value='level4' />");
		echo("</tr>");
			echo("</table>");
	//		echo("</form>");
        echo("</td>");
	echo("</tr>");
}
if ($add_this_object == 'department')
{
	
	/*
	 Get the parent company's name using the employer_id provided.
	*/

	$sql = ("
		SELECT DISTINCT company_name
		FROM employer_company
		WHERE employer_id='" . $employer_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	$company_name = $row["company_name"];
	
        echo("<tr>");
                echo("<td colspan='2'>");
                echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
                echo("<tr>");
                        echo("<td align='center' colspan='2' class='row0'>");
                        echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
                        echo("<tr>");
                                echo("<td align='center' colspan='2' nowrap='nowrap'><b class=black>Add a New Division:</b></td>");
                        echo("</tr>");
                        echo("<tr>");
                                echo("<td colspan='2'>&nbsp;</td>");
                        echo("</tr>");
			echo("<tr align='center'>");
				echo("<td colspan='2' align='center' nowrap='nowrap'>Parent Company:&nbsp;<b class='black'>" . $company_name . "</b></td>");
			echo("</tr>");
                        echo("<tr>");
                                echo("<td colspan='2'>&nbsp;</td>");
                        echo("</tr>");
                        echo("<tr>");
                                echo("<td align='right'>Division Name:</td>");
                                echo("<td align='left'><input type='text' name='department_name' size='50' maxlength='75' value='" . $department_name . "' /></td>");
                        echo("</tr>");
                        echo("<tr>");
                                echo("<td align='right'>Division Website:</td>");
                                echo("<td align='left'><input type='text' name='department_website' size='45' maxlength='60' value='" . $department_website . "' /></td>");
                        echo("</tr>");
                        echo("<tr>");
                                echo("<td nowrap='nowrap' align='right'>Division Description:</td>");
                                echo("<td align='left'><textarea name='department_description' rows='10' cols='40'>" . $department_description . "</textarea></td>");
                        echo("</tr>");
                        echo("</table>");
                        echo("</td>");
                echo("</tr>");
		echo("<tr>");
			echo("<td colspan='2' align='center'><input type='submit' value='Continue' />");
			echo("<input type='hidden' name='continue' value='level4' />");
		    echo("<input type='hidden' name='employer_id' value='" . $employer_id . "' /></td>");
        echo("</tr>");
     echo("</table>");
//echo("</form>");
  echo("</td>");
        echo("</tr>");
}
if ($add_this_object == 'contact')
{
	/*
	 Get the parent company and division (if there is one) names using the employer_id provided.
	*/

	$sql = ("
		SELECT DISTINCT company_name
		FROM employer_company
		WHERE employer_id='" . $employer_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	$company_name = $row["company_name"];

	if ($department_id && $department_id != 'none')
	{
		$sql = ("
			SELECT DISTINCT department_name
			FROM employer_department 
			WHERE department_id='" . $department_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		
		$department_name = $row["department_name"];
	}
	
        echo("<tr>");
                echo("<td colspan='2'>");
                echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
                echo("<tr>");
                        echo("<td align='center' colspan='2' class='row0'>");
                        echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
                        echo("<tr>");
                                echo("<td align='center' colspan='2' nowrap='nowrap'><b class=black>Add a New Contact:</b></td>");
                        echo("</tr>");
                        echo("<tr>");
                                echo("<td colspan='2'>&nbsp;</td>");
                        echo("</tr>");
			echo("<tr align='center'>");
				echo("<td colspan='2' align='center' nowrap='nowrap'>Parent Company:&nbsp;<b class='black'>" . $company_name . "</b></td>");
			echo("</tr>");
			if ($department_name)
			{
				echo("<tr align='center'>");
					echo("<td colspan='2' align='center' nowrap='nowrap'>Parent Division:&nbsp;<b class='black'>" . $department_name . "</b></td>");
				echo("</tr>");
			}
                        echo("<tr>");
                                echo("<td colspan='2'>&nbsp;</td>");
                        echo("</tr>");
                        echo("<tr>");
                                echo("<td nowrap='nowrap' align='right'>Contact's First Name:</td>");
                                echo("<td align='left'><input type='text' name='contact_first_name' size='22' maxlength='40' value='" . $contact_first_name . "' /></td>");
                        echo("</tr>");
			echo("<tr>");
				echo("<td nowrap='nowrap' align='right'>Contact's Last Name:</td>");
				echo("<td align='left'><input type='text' name='contact_last_name' size='22' maxlength='40' value='" . $contact_last_name . "' /></td>");
			echo("</tr>");
			echo("<tr>");
				echo("<td nowrap='nowrap' align='right'>Contact's E-mail:</td>");
				echo("<td align='left'><input type='text' name='contact_email' size='30' maxlength='100' value='" . $contact_email . "' /></td>");
			echo("</tr>");
			echo("<tr>");
				echo("<td nowrap='nowrap' align='right'>Contact's Phone:</td>");
				echo("<td align='left'><input type='text' name='contact_phone' size='22' maxlength='25' value='" . $contact_phone . "' /></td>");
			echo("</tr>");
            echo("<tr>");       
                echo("<td nowrap='nowrap' align='right'>Contact's Cellphone:</td>");
                echo("<td align='left'><input type='text' name='contact_cellphone' size='22' maxlength='25' value='" . $contact_cellphone . "' /></td>");
            echo("</tr>");   
			echo("<tr>");
				echo("<td nowrap='nowrap' align='right'>Contact's Fax:</td>");
				echo("<td align='left'><input type='text' name='contact_fax' size='22' maxlength='25' value='" . $contact_fax . "' /></td>");
			echo("</tr>");
                        echo("</table>");
                        echo("</td>");
                echo("</tr>");
                echo("<tr>");
                        echo("<td colspan='2' align='center'><input type='submit' value='Continue' />");
                        echo("<input type='hidden' name='continue' value='level4' /></td>");
                echo("</tr>");
	              //  echo("</form>");
                echo("</table>");
               	echo("<input type='hidden' name='employer_id' value='" . $employer_id . "' />");
		echo("<input type='hidden' name='department_id' value='" . $department_id . "' />");

                echo("</td>");
        echo("</tr>");
}
				
echo("</table>");
echo("</center>");
echo("</form>");
?>
