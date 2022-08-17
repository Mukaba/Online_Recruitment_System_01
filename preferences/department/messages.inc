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
 | Filename: messages.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

/*
<?php if (!(($client->browser == NETSCAPE) && ($client->platform == WIN))) { ?>
<script='text/javascript'   language="javascript">
document.loginform.loginID.focus();
</script>
<?php } ?>
*/
?>

<script type='text/javascript' language='javascript'>
<!--javascript
            
<?php 
    if($client->browser == NETSCAPE)
    {
?>
        var screen_width = "outerWidth";
        var screen_height = "outerHeight"; 
<?php
    }
    else
    {
?>
        var screen_width = "width";
        var screen_height = "height";
<?php
    }
?>
    
	function EmployerBodyPopup() 
    {
        window.open("mamook.php?select=editemployeremailpopup&no_headers=1", "EditEmployerEmailBody", 
                    "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,resizable=yes,"+screen_width+"=570,"+screen_height+"=560");
	}
    
    function DefaultTimeTablePopup() 
    {
        window.open("mamook.php?select=defaultsemesterspopup&no_headers=1", "DefaultSemesters", 
                    "toolbar=no,menubar=no,scrollbars=yes,fullscreen=0,top=0,left=0,resizable=yes,"+screen_width+"=550,"+screen_height+"=700"); 
    }
    
//-->
</script>


<h3 align="left">Preferences - Department</h3>

<?php
switch($submit)
{
    case "Save Changes":
        $result1 = $GLOBALS['dbh']->Execute("UPDATE department 
		                     SET general_instructions='".addslashes(stripslashes($general_instructions))."', 
		                     phone_instructions='".addslashes(stripslashes($phone_instructions))."',
		                     video_instructions='".addslashes(stripslashes($video_instructions))."',
		                     offcampus_instructions='".addslashes(stripslashes($offcampus_instructions))."',
		                     oncampus_instructions='".addslashes(stripslashes($oncampus_instructions))."',
		                     student_interview_instructions='".addslashes(stripslashes($student_interview_instructions))."',
		                     job_app_paper_instructions='".addslashes(stripslashes($job_app_paper_instructions))."',
		                     phone_assistance='".addslashes(stripslashes($phone_assistance))."',
		                     default_int_phone='".addslashes(stripslashes($default_int_phone))."',
		                     newsgroup_email='".addslashes(stripslashes($newsgroup_email))."',
		                     email='".addslashes(stripslashes($email))."',
		                     primary_contact_id='$primary_contact_id',
		                     placement_contact_id='$placement_contact_id',
		                     jobhost_contact_id='$jobhost_contact_id',
		                     shortlist_contact_id='$shortlist_contact_id',
		                     interview_contact_id='$interview_contact_id'
		                     WHERE department_id='". $auth->department ."'");
        success_check($result1, "department");
        notify("Changes have been successfully saved.");

    default:
        $sql = "SELECT general_instructions, phone_instructions, video_instructions, offcampus_instructions, 
                phone_assistance, default_int_phone, newsgroup_email, email, department_code, department_name,
                oncampus_instructions, student_interview_instructions, job_app_paper_instructions, primary_contact_id,
                placement_contact_id, jobhost_contact_id, shortlist_contact_id, interview_contact_id
                FROM department
                WHERE department_id='". $auth->department ."'";
        
        $result = $GLOBALS['dbh']->Execute($sql);
        while ($row=$result->FetchRow()) 
        {
            $general_instructions=$row['general_instructions'];
            $phone_instructions=$row['phone_instructions'];
            $video_instructions=$row['video_instructions'];
            $offcampus_instructions=$row['offcampus_instructions'];
            $oncampus_instructions=$row['oncampus_instructions'];
            $student_interview_instructions=$row['student_interview_instructions'];
            $job_app_paper_instructions=$row['job_app_paper_instructions'];
            $phone_assistance=$row['phone_assistance'];
            $default_int_phone=$row['default_int_phone'];
            $newsgroup_email=$row['newsgroup_email'];
            $email=$row['email'];
            $department_code=$row['department_code'];
            $department_name=$row['department_name'];
            $primary_contact_id=$row["primary_contact_id"];
            $placement_contact_id=$row["placement_contact_id"];
            $jobhost_contact_id=$row["jobhost_contact_id"];
            $shortlist_contact_id=$row["shortlist_contact_id"];
            $interview_contact_id=$row["interview_contact_id"];
        }        
?>

        <form method="post" action="<?php echo("$PHP_SELF&amp;select=prefer_dept&amp;level1=messages"); ?>">

        <table border="0" cellpadding="5" cellspacing="0" class="row1" width="95%">
        <tr>
            <td align="right">Department:</td>
            <td><b class="blue"><?php echo("$department_name ($department_code)") ?></b></td>
        </tr>
        <tr>
            <td align="right">General Interview Message:<br /><i>(displayed for all interview e-mails to employers)</i></td>
            <td><textarea name="general_instructions" rows="4" cols="50"><?php echo("$general_instructions") ?></textarea></td>
        </tr>
        <tr>
            <td align="right">Oncampus Interview Message:<br /><i>(appended after the general interview message)</i></td>
            <td><textarea name="oncampus_instructions" rows="4" cols="50"><?php echo("$oncampus_instructions") ?></textarea></td>
        </tr>
        <tr>
            <td align="right">Phone Interview Message:<br /><i>(appended after the general interview message)</i></td>
            <td><textarea name="phone_instructions" rows="4" cols="50"><?php echo("$phone_instructions") ?></textarea></td>
        </tr>
        <tr>
            <td align="right">Videoconference Interview Message:<br /><i>(appended after the general interview message)</i></td>
            <td><textarea name="video_instructions" rows="4" cols="50"><?php echo("$video_instructions") ?></textarea></td>
        </tr>
        <tr>
            <td align="right">Off-campus Interview Message:<br /><i>(appended after the general interview message)</i></td>
            <td><textarea name="offcampus_instructions" rows="4" cols="50"><?php echo("$offcampus_instructions") ?></textarea></td>
        </tr>
        <tr>
            <td align="right">Student Interview Message:<br /><i>(appended at the bottom of the interview notification e-mail sent to students)</i></td>
            <td><textarea name="student_interview_instructions" rows="4" cols="50"><?php echo("$student_interview_instructions") ?></textarea></td>
        </tr>
        <tr>
            <td align="right">Paper Job Application instructions:<br /><i>(appended at the bottom of paper job descriptions so students know where to deliver the paper applications)</i></td>
            <td><textarea name="job_app_paper_instructions" rows="4" cols="50"><?php echo("$job_app_paper_instructions") ?></textarea></td>
        </tr>
        <tr>
            <td align="right">Employer E-mail Message for Job Applications:<br /><i>(inserted into e-mail body when "Insert" is clicked on e-mail form in Job Applications)</i></td>
            <td><input type='button' name='employer_email_popup' value=' - Edit - ' onclick='EmployerBodyPopup()' /></td>
        </tr>
        <tr>
            <td align="right">Default Time Tables<br /><i>(edit the timetables set as default on Student Info -> Edit)</i></td>
            <td><input type='button' name='time_table_popup' value=' - Edit - ' onclick='DefaultTimeTablePopup()' /></td>
        </tr>
        <tr>
            <td align="right">Primary Department Contact:<br /><i>(miscellaneous messages from <?php echo(BRAND_NAME); ?>)</i></td>
            <td><select name='primary_contact_id'>

<?php
            $idsql = "SELECT CONCAT(c.first_name, ' ', c.last_name) as name, c.contact_id
                      FROM contact AS c
                      LEFT JOIN contact_internal AS ci ON c.contact_id = ci.contact_id
                      WHERE (ci.general_email OR c.contact_id='".$primary_contact_id."')
                      AND ci.department_id IN ('". implode (department_groups ($auth->department), "','")  ."')
                      ORDER BY c.last_name, c.first_name";

            $idresult = $GLOBALS['dbh']->Execute ($idsql);
            while ($idrow = $idresult->FetchRow()) 
            { 
                echo ("<option value='". $idrow["contact_id"] ."'". ($idrow["contact_id"] == $primary_contact_id ? " selected='selected'" : "" ). ">");
                echo ($idrow["name"] ."</option>");
            }
?>

            </select></td>
        </tr>
        <tr>
            <td align="right">Primary Job Host Contact:<br /><i>(24-hr holding notices sent to host department)</i></td>
            
            <td><select name='jobhost_contact_id'>

<?php
            $idsql = "SELECT CONCAT(c.first_name, ' ', c.last_name) as name, c.contact_id
                      FROM contact AS c, contact_internal AS ci
                      WHERE c.contact_id = ci.contact_id
                      AND (ci.general_email OR c.contact_id='".$jobhost_contact_id."')
                      AND ci.department_id IN ('". implode (department_groups ($auth->department), "','")  ."')
                      ORDER BY c.last_name, c.first_name";

            $idresult = $GLOBALS['dbh']->Execute ($idsql);
            
            while ($idrow = $idresult->FetchRow()) 
            { 
                echo ("<option value='". $idrow["contact_id"] ."'". ($idrow["contact_id"] == $jobhost_contact_id ? " selected='selected'" : "" ). ">");
                echo ($idrow["name"] ."</option>");
            }
?>

            </select></td>
        </tr>
        <tr>
            <td align="right">Primary Shortlisting Contact:<br /><i>(shortlisting notices sent by employer)</i></td>
            <td><select name='shortlist_contact_id'>

<?php
            $idsql = "SELECT CONCAT(c.first_name, ' ', c.last_name) as name, c.contact_id
                      FROM contact AS c, contact_internal AS ci
                      WHERE c.contact_id = ci.contact_id
                      AND (ci.general_email OR c.contact_id='".$shortlist_contact_id."')
                      AND ci.department_id IN ('". implode (department_groups ($auth->department), "','")  ."')
                      ORDER BY c.last_name, c.first_name";

            $idresult = $GLOBALS['dbh']->Execute ($idsql);
            while ($idrow = $idresult->FetchRow()) 
            { 
                echo ("<option value='". $idrow["contact_id"] ."'". ($idrow["contact_id"] == $shortlist_contact_id ? " selected='selected'" : "" ). ">");
                echo ($idrow["name"] ."</option>");
            }
?>

            </select></td>
        </tr>
        <tr>
            <td align="right">Primary Interview Contact:<br /><i>(notices regarding interviews)</i></td>
            
            <td><select name='interview_contact_id'>

<?php
            $idsql = "SELECT CONCAT(c.first_name, ' ', c.last_name) as name, c.contact_id
                      FROM contact AS c, contact_internal AS ci
                      WHERE c.contact_id = ci.contact_id
                      AND (ci.interview_contact_list OR c.contact_id = '".$interview_contact_id."')
                      AND ci.department_id IN ('". implode (department_groups ($auth->department), "','")  ."')
                      ORDER BY c.last_name, c.first_name";
        
            $idresult = $GLOBALS['dbh']->Execute ($idsql);
            while ($idrow = $idresult->FetchRow()) 
            { 
                echo ("<option value='". $idrow["contact_id"] ."'". ($idrow["contact_id"] == $interview_contact_id ? " selected='selected'" : "" ). ">");
                echo ($idrow["name"] ."</option>");
            }
?>

            </select></td>
        </tr>
        <tr>
            <td align="right">Primary Placement Contact:<br /><i>(contact for student-job matching)</i></td>
            <td><select name='placement_contact_id'>
            
<?php
            $idsql = "SELECT CONCAT(c.first_name, ' ', c.last_name) as name, c.contact_id
                      FROM contact AS c, contact_internal AS ci
                      WHERE c.contact_id = ci.contact_id
                      AND (ci.general_email OR c.contact_id = '".$placement_contact_id."')
                      AND ci.department_id IN ('". implode (department_groups ($auth->department), "','")  ."')
                      ORDER BY c.last_name, c.first_name";
        
            $idresult = $GLOBALS['dbh']->Execute ($idsql);
            while ($idrow = $idresult->FetchRow()) 
            { 
                echo ("<option value='". $idrow["contact_id"] ."'". ($idrow["contact_id"] == $placement_contact_id ? " selected='selected'" : "" ). ">");
                echo ($idrow["name"] ."</option>");
            }
?>

            </select></td>
        </tr>
        <tr>
            <td align="right">Local Phone Extension for Interview Assistance:</td>
            <td><input type="text" name="phone_assistance" size="40" value="<?php echo("$phone_assistance"); ?>"/></td>
        </tr>
        <tr>
            <td align="right">Default Phone Number for Phone Interviews:</td>
            <td><input type="text" name="default_int_phone" size="40" value="<?php echo("$default_int_phone"); ?>"/></td>
        </tr>
        <tr>
            <td align="right">Department Newsgroup:<br /><i>(optional - where student notifications will be sent)</i></td>
            <td><input type="text" name="newsgroup_email" size="40" value="<?php echo("$newsgroup_email"); ?>"/></td>
        </tr>
        <tr>
            <td align="right">Domain for Student E-mails<br /><i>(eg. uvic.ca)</i>:</td>
            <td><input type="text" name="email" size="40" value="<?php echo("$email"); ?>"/></td>
        </tr>
        <tr>
            <td colspan="2"><hr width="90%"/></td>
        </tr>
        <tr>
            <td align="center" colspan="2"><input type="submit" name="submit" value="Save Changes"/></td>
        </tr>    
        </table>
        
        </form>

<?php
}
?>
