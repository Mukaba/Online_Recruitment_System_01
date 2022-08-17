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
 | Filename: edit.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description: This displays a form in which the user can edit the resume.     |
 | In the case of a new resume the template resume is displayed for editing.    |
 +------------------------------------------------------------------------------+

*/


$error = 0;
if (($page != "Add"))
{
    // if we don't have a student number, but a resume id, we can figure out who the student is 
    if (!$student_num && $resume_id) 
    {
        $student_num_sql = ("
            SELECT student_number
            FROM resume
            WHERE resume_id = '".addslashes($resume_id)."'
            ");
        $student_num_result = $GLOBALS['dbh']->Execute($student_num_sql);
        $student_num_row = $student_num_result->FetchRow();
        $student_num = $student_num_row['student_number'];
    }

	if (!isValidResume($student_num, $resume_id) || !isValidStudentNumber($student_num, $department_in_str))
    {
		error("Please choose a specific resume.");
		include( "resume/admin/view_all.inc" );
		$error = 1;
	}
    
    else
    {
		//get resume
		$sql = "SELECT resume, name, type_id, DATE_FORMAT(last_modified, '%M %d, %Y (%l:%i %p)') as last_modified
                FROM resume 
                WHERE student_number = '" . addslashes($student_num) . "' 
				AND resume_id = '" . addslashes($resume_id) . "'";
	
		$result = $GLOBALS['dbh']->Execute( $sql );

		$row =  $result->FetchRow();
		$resume = $row["resume"];

        // IE's textarea's do not require find and replaces to preserve special HTML entities. 
        if ($client->browser != IE)
        {
            $resume = ereg_replace("&amp", "&amp;amp",$resume);    //preserves &amp; in textarea
            $resume = ereg_replace("&nbsp","&amp;nbsp",$resume);   //preserves &nbsp; in textarea
        }
		$last_modified = $row["last_modified"];
		$type_id = $row["type_id"];
		$name = $row["name"];
	}
}
else
{
	//we want to start with the template resume if it exists
	$sql = "SELECT resume
            FROM resume
            WHERE student_number = '" . addslashes($student_num) . "' AND type_id = '" . TEMPLATE . "'";
	
	$result = $GLOBALS['dbh']->Execute( $sql );
	$row =  $result->FetchRow();
	$resume = $row["resume"];
    if ($client->browser != IE)
    {
        $resume = ereg_replace("&amp", "&amp;amp",$resume);
        $resume = ereg_replace("&nbsp","&amp;nbsp",$resume);    
    }
	
}
if ($error == 0)
{
    echo("<table width=100%>");
    echo("<tr>");
    echo("<td><h3 align='left'>Student Information - Edit Resume</h3></td>\n");

    // Display the quickmenu
    echo("<td align='right'>");
    if ($record_id)
    {
        include('student_info/student_quickmenu.inc');
    }
    echo("</td>\n");
    echo("</tr>");
    echo("</table>");

	?>
    <b>Student:</b> <?php echo getStudentName($student_num) ?><br />
	<b>Student Number:</b> <?php echo $student_num?><br />
	<b>Resume:</b> <?php echo $name?><br />
	<b>Last Modified:</b> <?php echo (($last_modified=="") ? "Never" : $last_modified)?>
	<hr width="100%" />
	<form enctype="multipart/form-data" method="post" action="<?php echo ($PHP_SELF);?>">
	<input type="hidden" name="select" value="resume" />
	<input type="hidden" name="PDF" value="1" />
	<input type="hidden" name="department_id" value="<?php echo $department_id?>" />
	<input type="hidden" name="resume_id" value="<?php echo $resume_id?>" />
	<input type="hidden" name="type_id" value="<?php echo( $type_id ) ?>" />
	<input type="hidden" name="name" value="<?php echo( $name ) ?>" />
	<input type="hidden" name="student_num" value="<?php echo( $student_num ) ?>" />
    <input type="hidden" value="<?php echo($record_id)?>" name="record_id" />
    <input type="hidden" value="<?php echo($term)?>" name="term" />
    <input type="hidden" value="<?php echo($year)?>" name="year" />
    <input type="hidden" value="<?php echo($term_year_submit)?>" name="term_year_submit" />
	<table border="0" width="100%" cellspacing="0" cellpadding="5">
	  <tr>
	    <td>
	      <h4>Edit HTML:</h4>
	      <table border="0" width="100%" class="row1" cellpadding="5" cellspacing="0">
	        <tr>
		  <td align="center" width="100%"><textarea rows="30" cols="80" name="resume"><?php echo ($resume)?></textarea></td>
		</tr>
		<tr>
		  <td>
		    <table width="100%" border="0" cellpadding="0" cellspacing="0">
		      <tr>
		        <td width="50%" align="right"><input type="submit" value="Update Resume" name="page" />&nbsp;&nbsp;</td>
		        <td width="50%" align="left">&nbsp;&nbsp;<input type="reset" value="Revert" /></td>
		      </tr>
		    </table>
		   </td>
		</tr>
	      </table>
	    </td>
	  </tr>
<?php 
/*
	  <tr>
	    <td>
	      <h4>Update with File:</h4>
	      <table border="0" width="100%" class="row1" cellpadding="5" cellspacing="0">
	        <tr>
		  <td align="right">File:</td>
		  <td align="left"><input type="FILE" name="resume_file" size="30" /></td>
		</tr>
		<tr>
		  <td colspan="2"><input type="submit" value="Submit File" name="page" /></td>
		</tr>
	      </table>
	      <br /><hr width="100%"><br />
	    </td>
	  </tr>
*/
?>
	</table>
	
	</form>
<?php
}
?>
