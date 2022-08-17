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
 | Filename: contact.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

?>
<h3 align="left">Staff Info</h3>
<br /><br />

<?php

switch($submit)
  {
  case "Save Changes":

    $broken = 0;

    if (!isValidPhoneNumber ($phone) && strlen($phone))
    {
      error ("Phone number is invalid.");
      $broken = 1;
    }

    if (!isValidPhoneNumber ($fax) && strlen($fax))
    {
      error ("Fax number is invalid.");
      $broken = 1;
    }

    if (!strlen($first_name))
    {
      error ("First name is empty.");
      $broken = 1;
    }

    if (!strlen($last_name))
    {
      error ("Last name is empty.");
      $broken = 1;
    }

    if (!isValidEmail($email))
    {
      error ("E-mail address is invalid.");
      $broken = 1;
    }

    if (!strlen($netlink_id))
    {
      $netlink_id = "*";
    } else {
	    if (!isValidNetlink($netlink_id, $employee_number))
	    {
		error ("Netlink ID is invalid.");
		$broken = 1;
	    }
    }

    if (!strlen($nis_id))
    {
      $nis_id="*";
    } else {
	    if (!isValidNIS($nis_id, $employee_number))
	    {
		error ("Login ID is invalid.");
		$broken = 1;
	    }
    }

    if (!$broken && $new_id == 0)
    {
      $result1 = $GLOBALS['dbh']->Execute("UPDATE contact set first_name='".addslashes(stripslashes($first_name))."',
  			last_name='".addslashes(stripslashes($last_name))."',
			phone='".addslashes(stripslashes($phone))."',
			fax='".addslashes(stripslashes($fax))."',
			email='".addslashes(stripslashes($email))."'
			WHERE contact_id='$id'");
      success_check($result1, "contact");
      $result2 = $GLOBALS['dbh']->Execute("UPDATE contact_internal set department_id='$dept_id',
      			site_visit_list='$site_visit_list',
			interview_contact_list='$interview_contact_list',
			interview_lunch_list='$interview_lunch_list',
			application_replyto_list='$application_replyto_list',
			general_email='$general_email',
			login_id='".addslashes(stripslashes($nis_id))."',
			employee_number='".addslashes(stripslashes($employee_number))."',
			netlink_id='".addslashes(stripslashes($netlink_id))."'
			WHERE contact_id='$id'");
      success_check($result2, "contact_internal");
      notify("Changes have been successfully saved.");
    }
    else if (!$broken && $new_id == 1)
    {
      $GLOBALS['dbh']->Execute ("insert into contact (first_name, last_name, phone, fax, email, type)
                   values ('".addslashes(stripslashes($first_name))."', 
                           '".addslashes(stripslashes($last_name)). "',
                           '".addslashes(stripslashes($phone)).     "',
                           '".addslashes(stripslashes($fax)).       "', 
                           '".addslashes(stripslashes($email)).     "',
                           'internal')");

      $contact_id = $GLOBALS['dbh']->Insert_ID();

      $GLOBALS['dbh']->Execute ("insert into contact_internal (contact_id, department_id, site_visit_list, interview_contact_list, interview_lunch_list,
			application_replyto_list, general_email, login_id, employee_number, netlink_id)
                   values ('".addslashes(stripslashes($contact_id))            ."',
                           '".addslashes(stripslashes($dept_id))               ."', 
			   '".addslashes(stripslashes($site_visit_list))       ."',
                           '".addslashes(stripslashes($interview_contact_list))."', 
                           '".addslashes(stripslashes($interview_lunch_list))  ."',
			   '".addslashes(stripslashes($application_replyto_list))."',
			   '".addslashes(stripslashes($general_email))		."',
                           '".addslashes(stripslashes($nis_id))                ."',
                           '".addslashes(stripslashes($employee_number))       ."',
                           '".addslashes(stripslashes($netlink_id))            ."')");

      $new_id = 0;
      notify ("The new user has been added.");
    }

  default:

    if (strcmp($submit, "Save Changes") && !$new_id)
    {
      $sql = "SELECT c.first_name, c.last_name, c.phone, c.fax, c.email, ci.department_id, ci.site_visit_list,ci.interview_contact_list,
  		ci.login_id, ci.employee_number, ci.netlink_id, ci.interview_lunch_list, ci.application_replyto_list, ci.general_email
		FROM contact AS c, contact_internal AS ci
		WHERE c.contact_id=ci.contact_id AND c.contact_id='$id'"; 
      $result = $GLOBALS['dbh']->Execute($sql);
      while ($row=$result->FetchRow()) {
	$first_name=$row['first_name'];
	$last_name=$row['last_name'];
	$phone=$row['phone'];
	$fax=$row['fax'];
	$email=$row['email'];
	$dept_id=$row['department_id'];
	$site_visit_list=$row['site_visit_list'];
	$interview_contact_list=$row['interview_contact_list'];
	$nis_id=$row['login_id'];
	$employee_number=$row['employee_number'];
	$netlink_id=$row['netlink_id'];
	$interview_lunch_list=$row['interview_lunch_list'];
	$application_replyto_list=$row['application_replyto_list'];
	$general_email=$row['general_email'];
      }
    }

    ?>
    <form method="post" action="<?php echo("$PHP_SELF&amp;select=prefer_staff&amp;level1=contact"); ?>">
    <input type="hidden" name="id" value="<?php echo("$id"); ?>" />
    <input type="hidden" name="new_id" value="<?=$new_id?>" />
   
    <table border="0" cellpadding="5" cellspacing="0" class="row1" width="95%">
    	<tr>
    	<td align="right">First name:</td>
	<td><input type="text" name="first_name" size="40" value="<?php echo("$first_name"); ?>" /></td>
	</tr>
    	<tr>
    	<td align="right">Last name:</td>
	<td><input type="text" name="last_name" size="40" value="<?php echo("$last_name"); ?>" /></td>
	</tr>
	<tr>
	<td align="right">Department:</td>
	<td><select name='dept_id'>
	<?php
		$result = $GLOBALS['dbh']->Execute("SELECT department_id, department_name from department ORDER BY department_name");
		while( $row =  $result->FetchRow() )
		{
			$dept_name = $row['department_name'];
			$dept_all_id = $row['department_id'];
			if ($dept_all_id == $dept_id)
			{
				echo("<option selected='selected' value='$dept_all_id'>$dept_name</option>");
			} else {
				echo("<option value='$dept_all_id'>$dept_name</option>");
			}
		}
	?>
	</select></td>
	</tr>

    	<tr>
    	<td align="right">Phone:</td>
	<td><input type="text" name="phone" size="40" value="<?php echo("$phone"); ?>" /></td>
	</tr>
    	<tr>
    	<td align="right">Fax:</td>
	<td><input type="text" name="fax" size="40" value="<?php echo("$fax"); ?>" /></td>
	</tr>
    	<tr>
    	<td align="right">E-mail:</td>
	<td><input type="text" name="email" size="40" value="<?php echo("$email"); ?>" /></td>
	</tr>
    	<tr>
    	<td align="right">Netlink ID:</td>
	<td><input type="text" name="netlink_id" size="40" value="<?= !strcmp($netlink_id, "*") ? "" : $netlink_id ?>" /></td>
	</tr>
    	<tr>
    	<td align="right">NIS Login ID:</td>
	<td><input type="text" name="nis_id" size="40" value="<?= !strcmp($nis_id, "*") ? "" : $nis_id ?>" /></td>
	</tr>
    	<tr>
    	<td align="right">Employee number:</td>
	<td><input type="text" name="employee_number" size="40" value="<?php echo("$employee_number"); ?>" /></td>
	</tr>
        <tr>
        <td align="right">Name shown on the list for site visits?</td>
        <td><input class="row2" type="radio" name="site_visit_list" value="1" <?php echo (($site_visit_list == 1)? "checked='checked'" : "") ?> />Yes
        <input class="row2" type="radio" name="site_visit_list" value="0" <?php echo (($site_visit_list == 0)? "checked='checked'" : "") ?> />No</td>
        </tr>
        <tr>
        <td align="right">Name shown on the contact list for interviews?</td>
        <td><input class="row2" type="radio" name="interview_contact_list" value="1" <?php echo (($interview_contact_list == 1)? "checked='checked'" : "") ?> />Yes
        <input class="row2" type="radio" name="interview_contact_list" value="0" <?php echo (($interview_contact_list == 0)? "checked='checked'" : "") ?> />No</td>
        </tr>
        <tr>
        <td align="right">Name shown on the signup list for interview lunches?</td>
        <td><input class="row2" type="radio" name="interview_lunch_list" value="1" <?php echo (($interview_lunch_list == 1)? "checked='checked'" : "") ?> />Yes
        <input class="row2" type="radio" name="interview_lunch_list" value="0" <?php echo (($interview_lunch_list == 0)? "checked='checked'" : "") ?> />No</td>
        </tr>
        <tr>
        <td align="right">Name shown on the list for job applications?</td>
        <td><input class="row2" type="radio" name="application_replyto_list" value="1" <?php echo (($application_replyto_list == 1)? "checked='checked'" : "") ?> />Yes
        <input class="row2" type="radio" name="application_replyto_list" value="0" <?php echo (($application_replyto_list == 0)? "checked='checked'" : "") ?> />No</td>
        </tr>
        <tr>
        <td align="right">Name shown on the general email list?</td>
        <td><input class="row2" type="radio" name="general_email" value="1" <?php echo (($general_email == 1)? "checked='checked'" : "") ?> />Yes
        <input class="row2" type="radio" name="general_email" value="0" <?php echo (($general_email == 0)? "checked='checked'" : "") ?> />No</td>

        </tr>
	<tr><td colspan="2"><hr width="90%" /></td></tr>
	<tr><td align="center" colspan="2"><input type="submit" name="submit" value="Save Changes" /></td></tr>
    </table>
    </form>
<?php
  }
?>
