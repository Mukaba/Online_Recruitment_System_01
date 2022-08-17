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
 | Filename: edit_contact_form.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This form displays the contact information for the employer,    |
 | and allows them to edit it, with the exception of the contact name and       |
 | company name.                                                                |
 +------------------------------------------------------------------------------+

*/

?>

<script type="text/javascript" language="javascript">
<!--javascript

function renew()
{
	if (document.form1.new_department_id[document.form1.new_department_id.selectedIndex].value != document.form1.initial_department_id.value)
	{
		document.form1.javasubmit.value = 'true';
		document.form1.new_contact_id.selectedIndex = 0;
		document.form1.submit();
	}
}

function renew2()
{
	document.form1.javasubmit.value = 'true';
	if (document.form1.new_contact_id.selectedIndex != 0)
        {
                document.form1.enable_save.value='true';
        }
	document.form1.submit();
}

//-->

</script>

	<form name='form1' method="post" action="<?php echo($URL); ?>">
	<input type='hidden' name='job_id' value='<?php echo($job_id); ?>' />
	<input type='hidden' name='select' value='edit_job' />
	<input type='hidden' name='level1' value='contact' />	
	<input type='hidden' name='javasubmit' value='' />
	<input type='hidden' name='enable_save' value='' />
	<input type='hidden' name='new_employer_id' value='<?php echo($job->employer_id); ?>' />
	<input type='hidden' name='initial_department_id' value='<?php echo($job->department_id); ?>' />
	
	<center>
	<table border="0" cellpadding="5" cellspacing="0" width="100%" class="row1">

	<tr>
	        <td colspan='2'>
	        <br />
	        </td>
	</tr>

	<?php
	
	$sql = ("
		SELECT DISTINCT company_name, website, company_description
		FROM employer_company
		WHERE employer_id='" . $job->employer_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	$row = $result->FetchRow();
	
	$company_name = $row["company_name"];
	$company_website = $row["website"];
	$company_description = $row["company_description"];

	?>

	<tr>
	        <td width='50%' align="right" nowrap='nowrap'>Company Name:</td>
	        <td><?php echo($company_name); ?></td>
	</tr>

	<?php

	if ($company_website)
	{
		echo("<tr>");
		        echo("<td align='right' nowrap='nowrap'>Company Website:</td>");
		        echo("<td><a href='http://" . $company_website . "' target='employer_website'>" . $company_website . "</a>");
			echo("</td>");
		echo("</tr>");
	}
	if ($company_description)
	{
		echo("<tr>");
		        echo("<td align='right' nowrap='nowrap'>Company Description:</td>");
			echo("<td>" . $company_description . "</td>");
		echo("</tr>");
	}
	
	echo("<tr><td colspan='2'><br /></td></tr>");

	$sql = ("
		SELECT DISTINCT department_website, department_description
		FROM employer_department
		WHERE department_id='" . $job->department_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	$row = $result->FetchRow();
		
	$department_website = $row["department_website"];
	$department_description = $row["department_description"];

	$sql = ("
		SELECT DISTINCT ed.department_id, ed.department_name
		FROM employer_department AS ed, contact_employer AS ce
		WHERE ed.employer_id='" . $job->employer_id . "'
		AND ed.department_id=ce.department_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);


	echo("<tr>");
		echo("<td align='right' nowrap='nowrap'>Division Name:</td>");
		echo("<td>");

		echo("<input type='hidden' name='new_department_id' value='" . $job->department_id . "' />");
		echo(getDivisionName($job->department_id));

		/*
		 We're not letting the user edit their department anymore.

		echo("<select name='new_department_id' onchange='renew()'>");
		echo("<option value='none' ");
		if ($job->department_id == 'none')
		{
			echo("selected='selected'");
		}
		echo(">No Division</option>");
		while($row = $result->FetchRow())
		{
			echo("<option value='" . $row["department_id"] . "' ");
			if ($row["department_id"] == $job->department_id)
			{
				echo("selected");='selected'
			}
			echo(">" . $row["department_name"] . "</option>");
		}
		echo("</select>");
		*/

		echo("</td>");
	echo("</tr>");
	if ($department_website)
	{
		echo("<tr>");
			echo("<td align='right' nowrap='nowrap'>Division Website:</td>");
			echo("<td><a target='department_website' href='http://" . $department_website . "'>" . $department_website . "</a>");
			echo("</td>");
		echo("</tr>");
	}
	if ($department_description)
	{
		echo("<tr>");
			echo("<td align='right' nowrap='nowrap'>Division Description:</td>");
			echo("<td>" . $department_description . "</td>");
		echo("</tr>");
	}
	
	echo("<tr><td colspan='2'><br /></td></tr>");

	$sql = ("
		SELECT DISTINCT phone, email, fax, cellphone
		FROM contact
		WHERE contact_id='" . $job->contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	$row = $result->FetchRow();
    $contact_cellphone = $row["cellphone"];
	$contact_phone = $row["phone"];
	$contact_email = $row["email"];
	$contact_fax = $row["fax"];

	if ($job->department_id == 'none')
	{
		$sql = ("
        	        SELECT DISTINCT a.contact_id, CONCAT(b.first_name, ' ', b.last_name) AS name
        	        FROM contact_employer AS a, contact AS b
        	        WHERE a.employer_id='" . $job->employer_id . "'
			AND NOT (a.department_id)
        	        AND b.contact_id=a.contact_id
        	        AND b.first_name IS NOT NULL
        	        AND b.first_name!=''
        	        ");
	
	        $result = $GLOBALS['dbh']->Execute($sql);
	}
	else
	{
		$sql = ("
			SELECT DISTINCT a.contact_id, CONCAT(b.first_name, ' ', b.last_name) AS name
			FROM contact_employer AS a, contact AS b
			WHERE a.department_id='" . $job->department_id . "'
			AND a.employer_id='" . $job->employer_id . "'
			AND b.contact_id=a.contact_id
			AND b.first_name IS NOT NULL
			AND b.first_name!=''
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	}
	
	?>

	<tr>
        	<td align='right' nowrap='nowrap'>Contact Name:</td>
		<td><select name='new_contact_id'>
		<?php
        	// Put this line in place of the above one to allow for dynamic contact and division changing.
		// <td><select name='new_contact_id' language='javascript' onchange='renew2()' >

		/*
		 They will no longer be able to change to a blank contact.

		echo("<option value='' ");
		if (!$job->contact_id)
		{
			echo("selected='selected'");
		}
		echo(">&nbsp;</option>");
		*/

                while ($row = $result->FetchRow())
                {
                        echo("<option value='" . $row["contact_id"] . "' ");
                        if ($job->contact_id == $row["contact_id"])
                        {
                                echo("selected='selected'");
                        }
                        echo(">" . $row["name"] . "</option>");
                }
                echo("</select>");
		?>
        	</td>
	</tr>
	<?php
	if ($job->contact_id)
	{
		echo("<tr>");
		        echo("<td align='right' nowrap='nowrap'>Contact Phone:</td>");
		        echo("<td>" . (($contact_phone) ? $contact_phone : "&nbsp;"));
                echo("<input type='hidden' name='contact_phone' value='" . $contact_phone . "' />");
                echo("</td>");
		echo("</tr>");
        echo("<tr>");
                echo("<td align='right' nowrap='nowrap'>Contact Cell:</td>");
                echo("<td>" . (($contact_cellphone) ? $contact_cellphone : "&nbsp;"));
                echo("<input type='hidden' name='contact_cellphone' value='" . $contact_cellphone . "' />");
                echo("</td>");
        echo("</tr>");   
		echo("<tr>");
		        echo("<td align='right' nowrap='nowrap'>Contact E-mail:</td>");
		        echo("<td>" . (($contact_email) ? $contact_email : "&nbsp;"));
                echo("<input type='hidden' name='contact_email' value='" . $contact_email . "' />");
                echo("</td>");
		echo("</tr>");
		echo("<tr>");
		        echo("<td align='right' nowrap='nowrap'>Contact Fax:</td>");
		        echo("<td>" . (($contact_fax) ? $contact_fax : "&nbsp;"));
                echo("<input type='hidden' name='contact_fax' value='" . $contact_fax . "' />");
                echo("</td>");
		echo("</tr>");
	}
	?>
	<tr><td colspan='2'><br /></td></tr>
	<?php
	if ($enable_save)
	{
		echo("<tr>");
		        echo("<td align='center' colspan='2'>");
		        echo("<input type='submit' name='continue' value='Save Changes' />");
		        echo("</td>");
		echo("</tr>");
	}
	?>
	</table>
    </center>
	</form>
