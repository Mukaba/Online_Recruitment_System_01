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
| Filename: general.inc                                                        |
+------------------------------------------------------------------------------+
| Description:                                                                 |
+------------------------------------------------------------------------------+

*/

/*
* general.inc - General preferences for staff
*
*/
?>

<h3 align="left">Preferences - Personal: General Information</h3>


<?php

switch($submit)
{
case "Save Changes":
$result1 = $GLOBALS['dbh']->Execute("
    UPDATE contact_internal
    SET interview_contact_list='$interview_contact_list', 
    site_visit_list='$site_visit_list',
    email_signature='$email_signature',
    interview_lunch_list='$interview_lunch_list',
    pulldown_menu_group='$pulldown_menu_group',
    show_closed_cancelled='$show_closed_cancelled',
    send_sent_apps='$send_sent_apps',
    search_use_advanced='$search_use_advanced',
    menu_use_javascript='$menu_use_javascript',
    search_use_advanced_emp='$search_use_advanced_emp',
    search_use_advanced_hist='$search_use_advanced_hist',
    application_replyto_list='$application_replyto_list',
    employer_info_menu='$employer_info_menu'
    WHERE contact_id='$contact_id'");
success_check($result1, "contact_internal");



if ($_SESSION['SESSION_netlog']==SCRIPT_LOGIN){
    $idtype = 'netlink_id';
} else {
    $idtype = 'login_id';
}

$result_font = $GLOBALS['dbh']->Execute("
    UPDATE contact_internal
    SET user_font_size = '$user_font_size'
    WHERE $idtype = '$login_id'");
success_check($result_font, "contact_internal");




if($fontsize != $user_font_size){
    $font_notice = "<br /> Font size changes will not take effect until a new page is loaded";
} else $font_notice = "";
if ($has_java_enabled != $menu_use_javascript)
{
    $java_notice = "<br /> JavaScript Menu will not take effect until a new page is loaded";
} else $java_notice = "";
notify("Changes have been successfully saved." . $java_notice . $font_notice);

default:

if ($_SESSION['SESSION_netlog']==SCRIPT_LOGIN) 
{
    $sql = "SELECT a.contact_id, b.site_visit_list, b.email_signature, b.interview_contact_list, b.interview_lunch_list, b.menu_use_javascript, 
                   b.pulldown_menu_group, b.show_closed_cancelled, b.send_sent_apps, b.search_use_advanced, b.search_use_advanced_emp, b.search_use_advanced_hist, b.application_replyto_list,
                   b.employer_info_menu, b.user_font_size
            FROM contact AS a, contact_internal AS b 
            WHERE b.contact_id=a.contact_id 
            AND b.department_id='". $auth->department ."' 
            AND b.netlink_id='$login_id'";
} 
else 
{
    $sql = "SELECT a.contact_id, b.site_visit_list, b.email_signature, b.interview_contact_list, b.interview_lunch_list, b.menu_use_javascript,  
                   b.pulldown_menu_group, b.show_closed_cancelled, b.send_sent_apps, b.search_use_advanced, b.search_use_advanced_emp, b.search_use_advanced_hist, b.application_replyto_list,
                   b.employer_info_menu, b.user_font_size
            FROM contact AS a, contact_internal AS b 
            WHERE b.contact_id=a.contact_id 
            AND b.department_id='". $auth->department ."' 
            AND b.login_id='$login_id'";
}

$result = $GLOBALS['dbh']->Execute($sql);
while ($row=$result->FetchRow()) 
{
    $contact_id = $row['contact_id'];
    $site_visit_list = $row['site_visit_list'];
    $email_signature = $row['email_signature'];
    $interview_contact_list = $row['interview_contact_list'];
    $interview_lunch_list = $row['interview_lunch_list'];
    $pulldown_menu_group = $row['pulldown_menu_group'];
    $show_closed_cancelled = $row['show_closed_cancelled'];
    $send_sent_apps = $row['send_sent_apps'];
    $search_use_advanced = $row['search_use_advanced']; 
    $menu_use_javascript = $row['menu_use_javascript']; 
    $search_use_advanced_emp = $row['search_use_advanced_emp']; 
    $search_use_advanced_hist = $row['search_use_advanced_hist']; 
    $application_replyto_list = $row['application_replyto_list'];
    $employer_info_menu = $row['employer_info_menu'];
    $user_font_size = $row['user_font_size'];
}
?>

<form method="post" action="<?php echo("$PHP_SELF&amp;select=prefer&amp;level1=general"); ?>">

<input type="hidden" name="contact_id" value="<?php echo("$contact_id"); ?>" />
<table border="0" cellpadding="5" cellspacing="0" class="row1" width="95%">


<tr>
<td align="right" >Would you like to use the JavaScript Menu? (Note: You must have JavaScript enabled.)  </td>
<td nowrap='nowrap'><input class="row2" type="radio" name="menu_use_javascript" value="1" <?php echo (($menu_use_javascript == 1) ? "checked='checked'" : "") ?> />Yes </td>
<td colspan="2" nowrap='nowrap'><input class="row2" type="radio" name="menu_use_javascript" value="0" <?php echo (($menu_use_javascript == 0) ? "checked='checked'" : "") ?> />No </td>
    </tr>
<tr>
<td align="right" >What font size would you like to use? (Note: This font size will be used for all of your logins.)  </td>
<td nowrap='nowrap' colspan='3'><select name="user_font_size"> 
    <option value="xlarge"<?php if($user_font_size == 'xlarge') print " selected='selected'"; ?>>Extra-Large</option>
    <option value="large"<?php if($user_font_size == 'large') print " selected='selected'"; ?>>Large</option>
    <option value="normal"<?php if($user_font_size == 'normal' || empty($user_font_size)) print " selected='selected'"; ?>>Normal</option>
    <option value="small"<?php if($user_font_size == 'small') print " selected='selected'"; ?>>Small</option>
    <option value="xsmall"<?php if($user_font_size == 'xsmall') print " selected='selected'"; ?>>Extra-Small</option>
    </select>
</td>
</tr>


<?php
if (sizeof($departments_in_group) > 1)
{
?>
        <tr>
        <td align="right" >Would you like pulldown menus to default to your department or your entire group?</td>
        <td nowrap='nowrap'><input class="row2" type="radio" name="pulldown_menu_group" value="1" <?php echo (($pulldown_menu_group == 1)? "checked='checked'" : "") ?> />Group
        </td>
        <td colspan ="2" nowrap='nowrap'><input class="row2" type="radio" name="pulldown_menu_group" value="0" <?php echo (($pulldown_menu_group == 0)? "checked='checked'" : "") ?> />Department</td>
        </tr>

<?php
}
?>

<tr>
<td align="right" >What would you like as your Employer Info default menu?</td>
<td nowrap='nowrap'><input class="row2" type="radio" name="employer_info_menu" value="0" <?php echo (($employer_info_menu == 0) ? "checked='checked'" : "") ?> />Companies </td>
<td colspan="2" nowrap='nowrap'><input class="row2" type="radio" name="employer_info_menu" value="1" <?php echo (($employer_info_menu == 1) ? "checked='checked'" : "") ?> />Contacts </td>
    </tr>

<tr>
<td align="right" >When searching for job descriptions, would you like to default pulldown menus to show closed and cancelled jobs?</td>
<td nowrap='nowrap'><input class="row2" type="radio" name="show_closed_cancelled" value="1" <?php echo (($show_closed_cancelled == 1) ? "checked='checked'" : "") ?> />Yes </td>
<td colspan="2" nowrap='nowrap'><input class="row2" type="radio" name="show_closed_cancelled" value="0" <?php echo (($show_closed_cancelled == 0) ? "checked='checked'" : "") ?> />No </td>
    </tr>
<?php
if(!($auth->department == TRAFFIC_DIRECTOR_DEPARTMENT || $auth->department == DIRECTORS_OFFICE_DEPARTMENT))
{
?>
<tr>
<td align="right" >When viewing/sending applications, would you like to default Sent/Activated Applications to All, Yes, or No?</td>
<td nowrap='nowrap'><input class="row2" type="radio" name="send_sent_apps" value="2" <?php echo (($send_sent_apps == 2) ? "checked='checked'" : "") ?> />All </td>
<td nowrap='nowrap'><input class="row2" type="radio" name="send_sent_apps" value="1" <?php echo (($send_sent_apps == 1) ? "checked='checked'" : "") ?> />Yes </td>
<td nowrap='nowrap'><input class="row2" type="radio" name="send_sent_apps" value="0" <?php echo (($send_sent_apps == 0) ? "checked='checked'" : "") ?> />No </td>
    </tr>
<?php
}
?>
<tr><td colspan="4" align='center' nowrap='nowrap'>&nbsp;</td></tr>
<tr><td align="right" ><b class='black'>Would you like to use the simple or advanced search by default:</b></td><td colspan='3'>&nbsp;</td></tr>
    <tr>
    <td align="right" nowrap='nowrap'>When searching for students?</td>
    <td nowrap='nowrap'><input class="row2" type="radio" name="search_use_advanced" value="0" <?php echo (($search_use_advanced == 0) ? "checked='checked'" : "") ?> />Simple</td>
        <td colspan="2" nowrap='nowrap'><input class="row2" type="radio" name="search_use_advanced" value="1" <?php echo (($search_use_advanced == 1) ? "checked='checked'" : "") ?> />Advanced</td>
        </tr>
	
        <tr>
        <td align="right" >When searching for employers?</td>
        <td nowrap='nowrap'><input class="row2" type="radio" name="search_use_advanced_emp" value="0" <?php echo (($search_use_advanced_emp == 0) ? "checked='checked'" : "") ?> />Simple</td>
        <td colspan="2" nowrap='nowrap'><input class="row2" type="radio" name="search_use_advanced_emp" value="1" <?php echo (($search_use_advanced_emp == 1) ? "checked='checked'" : "") ?> />Advanced</td>
        </tr>
        
	<tr>
        <td align="right" >When searching for student placement history?</td>
        <td nowrap='nowrap'><input class="row2" type="radio" name="search_use_advanced_hist" value="0" <?php echo (($search_use_advanced_hist == 0) ? "checked='checked'" : "") ?> />Simple</td>
        <td colspan="2" nowrap='nowrap'><input class="row2" type="radio" name="search_use_advanced_hist" value="1" <?php echo (($search_use_advanced_hist == 1) ? "checked='checked'" : "") ?> />Advanced</td>
        </tr>

	<tr><td colspan="4" nowrap='nowrap' align='center'>&nbsp;</td></tr>
	<tr><td align="right" ><b class='black'>Would you like your name to be included:</b></td><td colspan='3'>&nbsp;</td></tr>

    	<tr>
    	<td align="right" >On the list for work site visits?</td>
    	<td nowrap='nowrap'><input class="row2" type="radio" name="site_visit_list" value="1" <?php echo (($site_visit_list == 1)? "checked='checked'" : "") ?> />Yes
	</td>
        <td colspan="2" nowrap='nowrap'><input class="row2" type="radio" name="site_visit_list" value="0" <?php echo (($site_visit_list == 0)? "checked='checked'" : "") ?> />No</td>
    	</tr>
    	<tr>
    	<td align="right" >On the contact list for interviews?</td>
    	<td nowrap='nowrap'><input class="row2" type="radio" name="interview_contact_list" value="1" <?php echo (($interview_contact_list == 1)? "checked='checked'" : "") ?> />Yes
	</td>
        <td colspan="2" nowrap='nowrap'><input class="row2" type="radio" name="interview_contact_list" value="0" <?php echo (($interview_contact_list == 0)? "checked='checked'" : "") ?> />No</td>
    	</tr>
    	<tr>
    	<td align="right" >On the signup list for interview lunches?</td>
    	<td nowrap='nowrap'><input class="row2" type="radio" name="interview_lunch_list" value="1" <?php echo (($interview_lunch_list == 1)? "checked='checked'" : "") ?> />Yes
	</td>
        <td colspan="2" nowrap='nowrap'><input class="row2" type="radio" name="interview_lunch_list" value="0" <?php echo (($interview_lunch_list == 0)? "checked='checked'" : "") ?> />No</td>
    	</tr>
	<tr>
	<td align="right" >On the 'Reply to' list for Application E-mails?</td>
	<td nowrap='nowrap'><input class="row2" type="radio" name="application_replyto_list" value="1" <?php echo (($application_replyto_list == 1)? "checked='checked'" : "") ?> />Yes
	</td>
	<td colspan="2" nowrap='nowrap'><input class="row2" type="radio" name="application_replyto_list" value="0" <?php echo (($application_replyto_list == 0)? "checked='checked'" : "") ?> />No</td>
	</tr>
	<tr><td colspan="4" nowrap='nowrap'><hr width="90%" /></td></tr>
	<tr>
    <td align="center" colspan="4"><b class='black'>E-mail signature:</b></td></tr>
    <tr><td align="center" colspan="4" nowrap='nowrap'><textarea name="email_signature" rows="6" cols="50"><?php echo("$email_signature") ?></textarea></td>
    </tr>

	<tr><td colspan="4" nowrap='nowrap'><hr width="90%" /></td></tr>
	<tr><td align="center" colspan="4" nowrap='nowrap'><input type="submit" name="submit" value="Save Changes" /></td></tr>
    </table>
    </form>

<?php
  }
?>
