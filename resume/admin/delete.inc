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
 | Description: Deletes resumes for a given student from the system.  Prompts   |
 | for confirmation from the user first.                                        |
 +------------------------------------------------------------------------------+

*/
if ($page = "View & Delete"){
	if ($confirm == "Yes"){
		$delete_count = 0;
		if (!isValidStudentNumber($student_num, $department_in_str)){
			error("Invalid Student.");
		}else{
			while(list($index, $resume) = each($resume_id)){
				if (!isValidResume($student_num, $resume)){
					$result = $GLOBALS['dbh']->Execute("SELECT name FROM resume WHERE resume_id = '" . addslashes($resume) . "'");
					$row = $result->FetchRow();
					error ("Invalid Resume - You cannot delete resume: " . $row["name"]);
				}else{
					$result = $GLOBALS['dbh']->Execute("SELECT count(*) as num_jobs  FROM coverletter WHERE resume_id = '" . addslashes($resume) . "' AND student_number = '" . addslashes($student_num) . "'");
					$row = $result->FetchRow();
					if ($row["num_jobs"] == 0){	
						$sql = "DELETE FROM resume 
							WHERE resume_id = '" . addslashes($resume) . "' 
							AND student_number = '" . addslashes($student_num) . "'";
						$result = $GLOBALS['dbh']->Execute($sql);
						if (!$result){
							error ("An error occured while trying to delete resume.");
						}else{
							$delete_count ++;
						}
					}else{
						$result = $GLOBALS['dbh']->Execute("SELECT name FROM resume WHERE resume_id = '" . addslashes($resume) . "'");
						$row = $result->FetchRow();
						error("You cannot delete resume " . $row["name"] . " until all coverletters that use this resume have been deleted for edited to no longer use this resume.");
					}
				}
			}
		}
		?><p><?php echo $delete_count?> resume(s) deleted for student <?php echo getStudentName($student_num)?>.</p>
		<?php
		include( "resume/admin/delete_main.inc" );
	
	}elseif ($level2 == "" || ($level2 == "Delete" && sizeof($resume_id) <= 0)){
		if ($level2 == "Delete" && sizeof($resume_id) <= 0){
			error("Please select at least one resume to delete");
		}
		
		$sql = "SELECT r.name, r.resume_id, DATE_FORMAT(r.last_modified, '%b %d, %Y') as last_modified, t.description
			FROM resume as r LEFT JOIN coverletter_type as t USING (type_id)
			WHERE r.student_number = '" . addslashes(stripslashes($student_num)) . "'";
		$result = $GLOBALS['dbh']->Execute($sql);

		?>
		<h3>Resume - Delete</h3>
		<?php
		if ($result->RecordCount() <= 0){
			?>
			<p><?php echo getStudentName($student_num)?> has no resumes in the system.</p>
			<?php
		}else{
			?>
			<table border="0" cellpadding="5" class='backgrey'>
			  <tr>
			    <td><p>Select which of the following resumes you wish to delete.  You will not be able to delete resumes that are attached to jobs.  You must delete the cover letter(s) first.</p>
			    </td>
			  </tr>
			</table>
			
			<form action="<?php echo $PHP_SELF?>" method="post">
			<input type="hidden" name="select" value="resume" />
			<input type="hidden" name="student_num" value="<?php echo $student_num?>" />
			<input type="hidden" name="page" value="<?php echo $page?>" />
			<input type="hidden" name="department_id" value="0" /> 
            <?php
            //allow them to delete any student's resume who is in their group echo $department_id">
            ?>
			<table border="1">
			  <tr>
			    <td class="rowgrey">&nbsp;</td>
			    <td class="rowgrey"><b class="white">Name</b></td>
			    <td class="rowgrey"><b class="white">Type</b></td>
			    <td class="rowgrey"><b class="white">Last Modified</b></td>
			    <td class="rowgrey"><b class="white">Jobs</b></td>
			  </tr>
			  <?php
			  $i = 0;
			  while ($row = $result->FetchRow()){
				 (($i % 2 == 0) ? $class="row0d" : $class="row1d");
				 $i++;
				 $sql = "SELECT j.job_code
				   	  FROM job_info as j, coverletter as c, resume as r
					  WHERE j.job_id = c.job_id 
						AND r.resume_id = c.resume_id 
						AND c.student_number = '" . addslashes($student_num) . "' 
						AND r.resume_id = '" . addslashes($row["resume_id"]) . "'";
				 $result2 = $GLOBALS['dbh']->Execute($sql);
				 $job_str = "";
				 $num_jobs = 0;
				 while ($row2 = $result2->FetchRow()){
					$job_str .= $row2["job_code"] . "<br />";
				 	$num_jobs ++;
				 }
				 $job_str = substr($job_str, 0, strlen($job_str) - 4);
				 ?>
				 <tr>
				   <?php
				   if ($num_jobs == 0){?>
				   <td class="<?php echo $class?>"><input type="checkbox" name="resume_id[]" value="<?php echo $row["resume_id"]?>" class="<?php echo $class?>" /></td>
				   <?php }else{?>
				   <td class="<?php echo $class?>">&nbsp;</td>
				   <?php }?>
				   <td class="<?php echo $class?>"><?php echo (($row["name"] == "") ? "&nbsp" : $row["name"])?></td>
				   <td class="<?php echo $class?>"><?php echo (($row["description"] == "") ? "&nbsp" : $row["description"])?></td>
				   <td class="<?php echo $class?>"><?php echo (($row["last_modified"] == "") ? "Never" : $row["last_modified"])?></td>
				   <td class="<?php echo $class?>"><?php echo (($job_str == "") ? "&nbsp;" : $job_str) ?></td>
				 </tr>
			<?php
			  }
			?>
			</table>
			<input type="submit" name="level2" value="Delete" />
			</form>
		<?php
		}
	}elseif ($level2 == "Delete" && sizeof($resume_id) > 0){
		?>
		<form method="post" action="<?php echo $PHP_SELF?>">
		<input type="hidden" name="department_id" value="<?php echo $department_id?>" />
		<input type="hidden" name="select" value="resume" />
		<input type="hidden" name="page" value="<?php echo $page?>" />
		<input type="hidden" name="student_num" value="<?php echo $student_num?>" />
		<?php
		while(list($index, $resume) = each($resume_id)){
			$in_str .= "'" . $resume . "', ";
			$sql = "SELECT name FROM resume WHERE resume_id = '" . addslashes($resume) . "'";
			$result = $GLOBALS['dbh']->Execute($sql);
			$row = $result->FetchRow();
			$name_str .= $row["name"] . ", ";
			?>
			<input type="hidden" name="resume_id[]" value="<?php echo $resume?>" />
			<?php
		}
		$in_str = substr($in_str, 0, strlen($in_str) - 2);
		$name_str = substr($name_str, 0, strlen($name_str) - 2);
		?>
		<table border="0" class='backgrey' cellpadding='5' width='100%'>
		  <tr>
		    <td><h4>Delete Resume Confirmation:</h4>
		      <p> Are you sure you want to delete resumes - <?php echo $name_str;?> - for <?php echo getStudentName($student_num);?>? </p>
		      <table align="center" border="0" width="100%" cellpadding="0" cellspacing="0">
		        <tr>
			  <td align="right">
			    <input type="submit" name="confirm" value="Yes" />
			    </form>
			  </td>
			  <td align="left">
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
}
?>
