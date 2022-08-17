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
 | Filename: getting_started.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file displays a list of frequently asked questions and     |
 | their associated answers                                                     |
 +------------------------------------------------------------------------------+

*/

$sql = ("
    SELECT c.first_name
    , c.last_name
    , c.email
    , c.phone 
    FROM contact AS c 
    INNER JOIN contact_internal AS ci ON ci.contact_id=c.contact_id 
    WHERE ci.traffic_director_email= 1
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$traffic = $result->FetchRow();
$traffic_name = $traffic['first_name'].' '.$traffic['last_name'];
$traffic_phone = $traffic['phone'];
$traffic_email = $traffic['email'];

$sql = ("
    SELECT department_id
    FROM job_info
    WHERE contact_id = ?
    ORDER BY date_entered DESC
    ");
$result = $GLOBALS['dbh']->Execute($sql, array($auth->contact_id));
if ($result->RecordCount() < 1) {
    $department_id_contact = DIRECTORS_OFFICE_DEPARTMENT;
} 
else {
    $department_contact = $result->FetchRow();
    $department_id_contact = $department_contact['department_id'];
}
$sql = ("
    SELECT c.first_name
    , c.last_name
    , c.email
    , c.phone 
    , d.department_name
    FROM contact AS c 
    INNER JOIN department AS d 
    ON d.primary_contact_id=c.contact_id
    WHERE d.department_id = ?
    ");
$result = $GLOBALS['dbh']->Execute($sql, array($department_id_contact));
$primary = $result->FetchRow();
$primary_name = $primary['first_name'].' '.$primary['last_name'];
$primary_phone = $primary['phone'];
$primary_email = $primary['email'];
$department_name = $primary['department_name'];
$sql = ("
    SELECT c.first_name
    , c.last_name
    , c.email
    , c.phone 
    FROM contact AS c 
    INNER JOIN department AS d 
    ON d.placement_contact_id=c.contact_id
    WHERE d.department_id = ? 
    ");
$result = $GLOBALS['dbh']->Execute($sql, array($department_id_contact));
$placement = $result->FetchRow();
$placement_name = $placement['first_name'].' '.$placement['last_name'];
$placement_phone = $placement['phone'];
$placement_email = $placement['email'];
$sql = ("
    SELECT c.first_name
    , c.last_name
    , c.email
    , c.phone 
    FROM contact AS c 
    INNER JOIN department AS d 
    ON d.shortlist_contact_id=c.contact_id
    WHERE d.department_id = ?
    ");
$result = $GLOBALS['dbh']->Execute($sql, array($department_id_contact));
$shortlist = $result->FetchRow();
$shortlist_name = $shortlist['first_name'].' '.$shortlist['last_name'];
$shortlist_phone = $shortlist['phone'];
$shortlist_email = $shortlist['email'];

?>

 </td><td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt="" /></td></tr></table>     

<table border="0" cellspacing="0" cellpadding="2" width="99%"><tr><td>
<h3>Getting Started</h3>
<ul>
<li><a href='#q1'>System Requirements</a></li>
<li><a href='#q2'>How do I see my Job Posting(s)?</a></li>
<li><a href='#q3'>How do I make changes to my Job Description(s)?</a></li>
<li><a href='#q4'>How do I change my Company/Contact information?</a></li>
<li><a href='#q5'>How do I view the Student Applications?</a></li>
<li><a href='#q6'>How do I shortlist candidates for Interviews?</a></li>
<li><a href='#q7'>How do I make a Job Offer to a student?</a></li>
<li><a href='#q8'>How do I post another Job?</a></li>
<li><a href='#q9'>How do I change my Password?</a></li>
<li><a href='#q10'>Who do I contact for more Assistance?</a></li>
</ul>
<h4><a name='q1'>System Requirements</a></h4>
<p>Our system uses the Portable Document File (PDF) format to view applications. If you do not have the Adobe Acrobat PDF Reader 
installed on your computer, you can download it for free from Adobe's web site located at: 
<a href='http://www.adobe.com/products/acrobat/readstep2.html' target="_blank">http://www.adobe.com/products/acrobat/readstep2.html</a> (a new window will open for you). When installing, make sure that you configure 
the installation to your browser.</p>
<p>If you have quite a few applications, it may take a few minutes to load them if you do not have a high-speed internet connection. 
If you have a problem accessing the applications online, please contact <?php echo($traffic_name); echo(" (".$traffic_phone.", ".$traffic_email.")"); ?> and other arrangements will be made to provide you 
with the resume packages.</p>
<h4><a name='q2'>How do I see my Job Posting(s)?</a></h4>
<p>Click on "View Jobs" from the menu on the left. You will see a list of the jobs posted by your company for this term. 
Select a job by clicking on its hyper-links and you will see the job description.</p>
<p>If you don't see your job it may be because it was posted in a different term from the one the system is currently showing. 
You can select the term you would like to see (or all terms) in the pull-down menu at the top of the page.</p>
<h4><a name='q3'>How do I make changes to my Job Description(s)?</a></h4>
<p>If your job posting is currently open to students, you will not be able to make any changes to the posting yourself. 
If you need to make a change, contact your regular program contact or the Employer Service Coordinator.</p>
<p>You may make changes to your job posting before it is viewable by students by clicking on "Edit a Job" from the menu on the left.</p>
<h4><a name='q4'>How do I change my Company/Contact information?</a></h4>
<p>If you would like to make a change to your own record or your company's, contact your regular program contact or the 
Employer Service Coordinator.</p>
<h4><a name='q5'>How do I view the Student Applications?</a></h4>
<p>Click on "Student Applications" from the menu on the left. All jobs posted by your organization for the upcoming term 
will be viewable in a list. To see the applications for any job just click on the job. You will be able to see student 
applications only after the job's closing date.</p>
<p>You can also view applications from the previous two years by choosing the appropriate term from the pulldown menu near 
the top of the screen.</p>
<h4><a name='q6'>How do I shortlist candidates for Interviews?</a></h4>
<p>You may shortlist online through Student Applications, or e-mail or phone your program contact, <?php echo($shortlist_name); echo(" (".$shortlist_phone.", ".$shortlist_email.")"); ?>.</p>
<h4><a name='q7'>How do I make a Job Offer to a student?</a></h4>
<p>Contact <?php echo($placement_name); echo(" (".$placement_phone.", ".$placement_email.")"); ?> in the <?php echo($department_name); ?> Co-op Program. They will advise the student of your choice 
of the offer and give you their response within 24 hours.</p>
<h4><a name='q8'>How do I post another Job?</a></h4>
<p>Click on "Add a Job" from the menu on the left. If you've posted a job before with us you can use this as a template for your 
new job, or start from scratch.</p>
<h4><a name='q9'>How do I change my Password?</a></h4>
<p>Just click on "Change Password" from the menu on the left and follow the instructions.</p>
<h4><a name='q10'>Who do I contact for more Assistance?</a></h4>
<p>Contact <?php echo($primary_name); echo(" (".$primary_phone.", ".$primary_email.")"); ?> in the <?php echo($department_name); ?> 
Co-op Program, or our Employer Service Coordinator, <?php echo($traffic_name); echo(" (".$traffic_phone.", ".$traffic_email.")"); ?> </p>

</td></tr></table>
