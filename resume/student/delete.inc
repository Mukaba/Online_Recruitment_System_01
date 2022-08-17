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
 | Filename: delete.inc                                                         |
 +------------------------------------------------------------------------------+
 | Description: Confirms that the resume the user selected to delete is         |
 | valid, confirms they want to delete it, deletes it.                          |
 +------------------------------------------------------------------------------+

*/

if ($userlevel == ADMINSTUDENT) {
    error("<h1 class=\"black\">Error</h1><p>You are not allowed to delete a student's resume.</p>");
}
elseif ($confirm == "Yes")
{
	
	//if the user has confirmed they want to delete their resume then check to ensure the resume_id is valid
	//(so their not deleting other peoples resumes) and then delete resume
	
	if (!isValidResume($student_number, $resume_id)){
		error ("Invalid Resume - You cannot delete this resume");
		include ("resume/student/delete_main.inc");
	}
	else if ($userlevel == ADMINSTUDENT)
	{
		error ("You cannot delete resumes while logged in as an administrator.");
		include ("resume/student/delete_main.inc");
	}else{
		$result = $GLOBALS['dbh']->Execute( "SELECT count(*) as job_count FROM coverletter WHERE resume_id = '" . addslashes($resume_id) . "'");
		$row = $result->FetchRow();
		if ($row["job_count"] <= 0){
			$sql = "DELETE FROM resume 
				WHERE resume_id = '" . addslashes($resume_id) . "' 
					AND student_number = '" . addslashes($student_number) . "'";
			$result = $GLOBALS['dbh']->Execute($sql);
			if (!$result){
				error ("An error occured while trying to delete resume.");
			}else{
				echo ("<b>Your resume has been deleted.</b>");
			}
		}else{
			error("You cannot delete a resume that has a job associated with it.");
		}
		include( "resume/student/delete_main.inc" );
	}
}else{
	//confirm the user wants to delete the resume they selected
	
	$sql = "SELECT name 
		FROM resume 
		WHERE resume_id = '" . addslashes($resume_id) . "' 
			AND student_number = '" . addslashes($student_number) . "'";
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$name = $row["name"];
	?>
	<table class='backgrey' cellpadding='5' width='100%'>
	  <tr>
	    <td>
	      <h4>Delete Resume Confirmation:</h4>
	      <p>Are you sure you want to delete resume <?php echo $name?>.</p>
	      <table cellpadding="0" cellspacing="0" align="center">
	        <tr>
		  <td>
		    <form action="<?php echo $PHP_SELF?>" method="post">
		    <input type="hidden" name="select" value="resume" />
		    <input type="hidden" name="page" value="Delete" />
		    <input type="hidden" name="resume_id" value="<?php echo $resume_id?>" />
		    <input type="submit" name="confirm" value="Yes" />
		    </form>
		  </td>
		  <td>
		    <form action="<?php echo $PHP_SELF?>" method="post">
		    <input type="hidden" name="select" value="resume" />
		    <input type="hidden" name="page" value="delete" />
		    <input type="submit" name="confirm" value="No" />
		    </form>
		  </td>
		</tr>
 	      </table>
	    </td>
	  </tr>
	</table>
	<?php

}
?>
		    
		
