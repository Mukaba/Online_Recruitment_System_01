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
 | Filename: add.inc                                                            |
 +------------------------------------------------------------------------------+
 | Description: This page has been set up so the user can either enter a        |
 | coverletter for a single student and single job or enter coverletters for    |
 | multiple students for a single job by using the student generic              |
 | coverletters and resumes. For each of the student names enter the            |
 | corresponding student number is found.  If there are multiple students with  |
 | the same name the user is prompted to select which student they want by      |
 | student number.  Once all student numbers are found it ensures the student   |
 | has not already applied to the job and then either brings up the edit.inc    |
 | file (in the case where generic coverletters are not being used) or the      |
 | save.inc file in the case where generic coverletters and resumes are being   |
 | used.                                                                        |
 +------------------------------------------------------------------------------+

*/

if (isset($student_serialized)) {
	$students = unpackObject(($student_serialized));
} else { 
	$students = explode("\n", trim($student));
}

$job_code = urldecode(stripslashes($job_code));

$error = "";
if (trim($job_code) == "" && $add_info == ""){
	$error = ("Invalid Job Code. ");
}elseif (trim($student) == "" && $add_info == ""){
	$error = ("Please enter at least one student");
}elseif ($student != "" || $student_serialized != ""){
	if (sizeof($students)>1 && $use_generic != "on"){
		$error = "If you are adding cover letters for multiple students you must use their generic cover letters and resumes.";
	}else{
		$coverletter_add = new AddAdminCoverletter(NULL, $students, sizeof($students), $job_code, $job_id, $use_generic, 0);
	}
}elseif($add_info != ""){
	$add_info = unpackObject(($add_info));
	$coverletter_add = new AddAdminCoverletter("", "", "", "", "", "", "");
	$coverletter_add->restore_coverletter($add_info);
	if ($job_id != ""){
		$coverletter_add->job_id = $job_id;
	}
}
if ( $error != "" || !isset($coverletter_add) ){
	if ($error != ""){
		error($error);
	}else{
		error ("Object not created");
	}
	include("coverletters/admin/add_main.inc");
}else{
	$str = "";
	if ($coverletter_add->job_id == ""){
		$str = $coverletter_add->getJobCode($department_in_str);
	}
	if ($str != "false" && $str != "true" && $str != ""){
		?>
		<form name="form1" method="post" action="<?php echo $PHP_SELF."&amp;job_code=$job_code&amp;add_info=".(packObject(($coverletter_add))); ?>">
		<input type="hidden" name="department_id" value="<?php echo $coverletter_add->department_id?>" />
		<input type="hidden" name="add_info" value="<?php echo packObject(($coverletter_add))?>" />
		<input type="hidden" name="student_serialized" value="<?php echo packObject(($students))?>" />
		<?php echo $str;
		?>
		</form>
		<?php
	}elseif($str == "false"){
		error ("Invalid Job Code or Job Code not in your department");
		include("coverletters/admin/add_main.inc");
	}elseif($str == "true" || $str == "") {
		if ($student_num != ""){
			if (check_existance($student_num, $coverletter_add->job_id)){
				$coverletter_add->applied_students[] = getStudentName($student_num);
			}else{
				$coverletter_add->student_numbers[] = $student_num;
			}
		}
		if (sizeof($coverletter_add->applied_students) == 0 && sizeof($coverletter_add->invalid_students) == 0 && sizeof($coverletter_add->student_numbers) == 0 && sizeof($coverletter_add->duplicate_names) == 0){
			$coverletter_add->getStudentNumbers($department_in_str);
		}
		if (sizeof($coverletter_add->duplicate_names) > 0){
			list($first, $last) = explode(" ", array_pop($coverletter_add->duplicate_names));	
			include("misc/names.inc");
			select_name($first, $last, $PHP_SELF . "&amp;select=coverletter&amp;page=Add+Cover+Letter", array(array("add_info", packObject(($coverletter_add)))));
		
		}else{
			if (sizeof($coverletter_add->invalid_students) > 0){
				foreach($coverletter_add->invalid_students as $invalid_name){
					error("Student $invalid_name does not exist in the database.");
				}
			}
			if (sizeof($coverletter_add->applied_students) > 0){
				foreach($coverletter_add->applied_students as $applied_student){
					error("Student $applied_student has already applied to this job.  To edit their cover letter please use the edit menu.");
				}
			}
			if(sizeof($coverletter_add->student_numbers) > 0){
				if($coverletter_add->num_students == 1 && $coverletter_add->use_generic != "on"){
					$student_num = $coverletter_add->student_numbers[0];
					$sql = "SELECT name FROM resume WHERE student_number = '" . addslashes($student_num) . "'";
					$result = $GLOBALS['dbh']->Execute($sql);
					if ($result->RecordCount() <= 0){
						error("Student " . getStudentName($student_num) . " has no resumes in the system. You can not add a cover letter letter for them.");
						include("coverletters/admin/add_main.inc");
					}else{
						$job_id = $coverletter_add->job_id;
						$department_id = $coverletter_add->department_id;
						include("coverletters/admin/edit.inc");
					}
				}else{
					$student = $coverletter_add->student_numbers[0];
					$students = $coverletter_add->student_numbers;
					$job_id = $coverletter_add->job_id;
					$type_id = SPECIFIC;
					$use_generic = $coverletter_add->use_generic;
					$department_id = $coverletter_add->department_id;
					include("coverletters/save.inc");
					$student = "";
					$student_num = "";
					include("coverletters/admin/view_results.inc");
				}
			}else{
				error("There are no students to add.");
				include("coverletters/admin/add_main.inc");
			}

		}
	}
}

//returns true if a coverletter already exists for the student and job false otherwise
function check_existance($student, $job_id){

	$sql = "SELECT letter 
		FROM coverletter 
		WHERE student_number = '" . addslashes(stripslashes($student)) . "' 
			AND job_id = $job_id AND type_id = " . SPECIFIC;
	$result = $GLOBALS['dbh']->Execute($sql);
	if ( $result->RecordCount() <= 0){
		return false;
	}else{
		return true;
	}
}
?>
