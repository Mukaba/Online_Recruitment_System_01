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
 | Filename: builder.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description: The Cover letter Builder tries to fill in as many fields as     |
 | possible, then lets the user fill in the rest.  It then converts it to       |
 | html, using the pages stylesheets. To convert to html, it sends the fields   |
 | to buildhtml.inc, which does the rest of the work.                           |
 +------------------------------------------------------------------------------+

*/



// Pull out user Info

// Student Info
$sql = "SELECT first_name, last_name, street_address_current, postal_code_current, city_current, province_current FROM student WHERE login_id = '".$_SESSION['SESSION_loginID']."'";
if ($get_student = $GLOBALS['dbh']->Execute($sql)) {
	if ($row = $get_student->FetchRow()) {
		$name = $row["first_name"] . " " . $row["last_name"];

		// get province : 
		$province = "";
		if ($get_province = $GLOBALS['dbh']->Execute("select provstate_name from provstate_list where provstate_id = '". $row["province_current"] . "'")) {
			if ($row_prov = $get_province->FetchRow()) {
				$province = $row_prov["provstate_name"];
			}
		}
		$current_address = $row["street_address_current"] . "\n" . $row["city_current"] . "," . $province . "\n" . $row["postal_code_current"];

	}
}

/* This has been removed , maybe it'll   come back, maybe it won't
// Employer Info
$sql = "SELECT x.first_name, x.last_name, x.title, e.company_name, e.street_address_1, e.street_address_2, e.city, e.provstate_id, e.country_id, e.postal_code, e.phone, e.fax
	FROM employer_company as e, job_info as i, contact as x where i.job_id = '$job_id' and i.employer_id = e.employer_id and i.contact_id = x.contact_id";
if ($get_employer_info = $GLOBALS['dbh']->Execute($sql)) {
	if ($row = $get_employer_info->FetchRow()) {
		$company_name = $row["company_name"];
		$contact_name = $row["first_name"] . " " . $row["last_name"];
		$contact_title = $row["title"];

		$get_province = $GLOBALS['dbh']->Execute("Select p.provstate_name from provstate_list as p where p.provstate_id = '". $row["provstate_id"] . "'");
		if ($row2 = $get_province->FetchRow()) { 
			if ($row["provstate_id"]) { $province = $row2["provstate_name"]; }
		}
		
		$get_country = $GLOBALS['dbh']->Execute("select c.country_name from country_list as c where c.country_id = '". $row["country_id"]."'");
		if ($row3 = $get_country->FetchRow()) {
			if ($row["country_id"])	{ $country = $row3["country_name"]; }
		}

		if (strlen($row["$street_address_1"])) { $street_address = $row["street_address_1"] . "\n" . $row["street_address_2"] . "\n"; } else { $street_address = "";} 
		if (strlen($row["city"]) OR strlen($province) OR strlen($country)) { $area = $row["city"] . ", " . $province . ", " . $country . "\n"; } else { $area = ""; }
		if (strlen($row["postal_code"])) { $postal_code = $row["postal_code"] . "\n"; } else { $postal_code = ""; }
		$company_address = $street_address . $area . $postal_code .  $row["phone"];


	}
}
*/


?>



<table border="0" cellspacing="2" cellpadding="0" width="100%">
  <tr>
    <td><h3 align="left">Cover Letter Builder</h3><hr width="100%" /></td>
  </tr>
  <tr>
    <td>
	<form method="post" action="<?php echo $PHP_SELF?>">
	<input type="hidden" name="select" value="coverletter" />
	<input type="hidden" name="page" value="build" />
	<input type="hidden" name="coverletter_name" value="<?php echo $coverletter_name?>" />
	<input type="hidden" name="job_code" value="<?php echo $job_code?>" />
	<input type="hidden" name="job_id" value="<?php echo $job_id?>" />
	<input type="hidden" name="type_id" value="<?php echo $type_id?>" />
	<input type="hidden" name="resume_id" value="<?php echo $resume_id?>" />

	<table border=0 cellpadding="5" cellspacing="0" width="100%" class="row1">
	  <tr><td>
	     		Your Name : </td><td><input type="text" name="name" value="<?php echo stripslashes("$name")?>" size="30" maxlength="100" />
	 </td></tr>
	  <tr><td>
	     		Your Address : </td><td><textarea name="current_address" rows="4" cols="60"><?php echo stripslashes("$current_address")?></textarea>
	 </td></tr>
	
		<?php	/* this has been removed for the time being
		
			// Only Display Employer Information if this is for a specific Job.
			 //if (($job_id) || ($job_code != "Template" && $job_code != "Generic")) { ?>
		<tr><td colspan='2'><hr /></td></tr> 

		<tr><td colspan='2'>Employer Information :</td></tr>
		<tr><td>
			Name of Company : </td><td><input type="text" name="company_name" value="<?php echo stripslashes("$company_name")?>" size="30" maxlength="100" />
		</td></tr>
		<tr><td>
			Name of Contact : </td><td><input type="text" name="contact_name" value="<?php echo stripslashes("$contact_name")?>" size="30" maxlength="100" />
		</td></tr>
		<tr><td>
			Title of Contact : </td><td><input type="text" name="contact_title" value="<?php echo stripslashes("$contact_title")?>" size="30" maxlength="100" />
		</td></tr>
		<tr><td>
			Address :	  </td><td><textarea name="company_address" rows="4" cols="60"><?php echo stripslashes("$company_address")?></textarea>
		</td></tr>

		<?php }*/ ?>

	<tr><td colspan='2'><hr /></td></tr> 

	<tr><td> Salutation : </td>
		<td><input type="text" name="salutation" value="Dear Mr./Ms., " /></td></tr>

	<tr><td colspan="2"> Type In cover letter (Normal Text, no HTML required) : <br />
		<?php// if ($client->platform == WIN) { ?>
		<i>(if the tab key doesn't work, type <b class='black'>-tab-</b> to enter a tab.)<br /> 
		<?php //} ?>
		<textarea name="coverletter" rows="20" cols="80"></textarea>
	</td></tr>

	</table>		

    </td>
  </tr>
  <tr>
    <td>
  	<input type="hidden" value="1" name="PDF" /><input type="submit" name="page" value="Create HTML Cover Letter" />
	
	<input type="RESET" value="Clear Form" />
    </td>
  </tr>
</form>
</table>

