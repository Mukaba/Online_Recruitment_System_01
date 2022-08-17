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
<h3 align="left">Preferences - Personal: Contact Information</h3>

<?php

switch($submit)
  {
  case "Save Changes":
    $result2 = $GLOBALS['dbh']->Execute("
		UPDATE contact SET first_name='$first_name',
		last_name='$last_name', email='$email', 
		phone='$phone', fax='$fax'
		WHERE contact_id='$contact_id'");
    success_check($result2, "contact");
    if ($auth->userlevel == FACULTY)
    {
        $result3 = $GLOBALS['dbh']->Execute("
            UPDATE contact_internal SET email_signature='$email_signature'
            WHERE contact_id='$contact_id'");
        success_check($result3, "contact_internal");
    }
    notify("Changes have been successfully saved.");

  default:
//    echo("<h4>Contact Info:</h4>");
    if ($_SESSION['SESSION_netlog']==SCRIPT_LOGIN) {
    $sql = "SELECT a.contact_id, a.first_name, a.last_name, a.email, a.phone, a.fax, b.email_signature
    	FROM contact AS a, contact_internal AS b WHERE b.contact_id=a.contact_id 
    	AND b.department_id='$auth->department' AND b.netlink_id='$login_id'";
    } else {
    $sql = "SELECT a.contact_id, a.first_name, a.last_name, a.email, a.phone, a.fax, b.email_signature
    	FROM contact AS a, contact_internal AS b WHERE b.contact_id=a.contact_id 
    	AND b.department_id='$auth->department' AND b.login_id='$login_id'";
    }
    $result = $GLOBALS['dbh']->Execute($sql);
    while ($row=$result->FetchRow()) {
	$contact_id=$row['contact_id'];
    	$first_name=$row['first_name'];
    	$last_name=$row['last_name'];
    	$email=$row['email'];
    	$phone=$row['phone'];
    	$fax=$row['fax'];
        $email_signature=$row['email_signature'];
    }
    $sql = "SELECT department_name FROM department WHERE department_id=$auth->department";
    $result = $GLOBALS['dbh']->Execute($sql);
    while ($row=$result->FetchRow()) {
    	$department_name = $row['department_name'];
    }
    ?>
            <form method="post" action="<?php echo("$PHP_SELF&amp;select=prefer&amp;level1=contact"); ?>">
    
            <input type="hidden" name="contact_id" value="<?php echo("$contact_id"); ?>" />
            <table border="0" cellpadding="5" cellspacing="0" class="row1" width="90%">
            <tr>
            <td align="right">Department:</td>
            <td align="left"><b class='blue'><?php echo("$department_name"); ?></b></td>
            </tr>
            <tr>
            <td align="right">First Name:</td>
            <td><input type="text" name="first_name" value="<?php echo("$first_name"); ?>" size="20" maxlength="50" /></td>
            </tr>
            <tr>
            <td align="right">Last Name:</td>
            <td><input type="text" name="last_name" value="<?php echo("$last_name"); ?>" size="20" maxlength="50" /></td>
            </tr>
            <tr>
            <td align="right">Phone:</td>
            <td><input type="text" name="phone" value="<?php echo("$phone"); ?>" size="20" maxlength="50" /></td>
            </tr>
            <tr>
            <td align="right">Fax:</td>
            <td><input type="text" name="fax" value="<?php echo("$fax"); ?>" size="20" maxlength="50" /></td>
            </tr>
            <tr>
            <td align="right">E-mail:</td>
            <td><input type="text" name="email" value="<?php echo("$email"); ?>" size="40" maxlength="50" /></td>
            </tr>
    <?php
    if ($auth->userlevel == FACULTY)
    {
        echo("<tr><td colspan='2' nowrap='nowrap'><hr width='90%' /></td></tr>");
        echo("<tr><td colspan='2' align='center'><b class='black'>E-mail signature:</b></td></tr>");
        echo("<tr><td colspan='2' align='center' nowrap='nowrap'><textarea name='email_signature' rows='6' cols='50'>$email_signature</textarea></td>");
    }
    ?>
    <tr><td colspan="2"><hr width="90%" /></td></tr>
	<tr><td align="center" colspan="2"><input type="submit" name="submit" value="Save Changes" /></td></tr>
    	</table>
    </form>
<?php
  }
?>
