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
 | Description: Based on the information select from delete_main.inc this       |
 | file prompts the user for confirmation of deletion and then updates the      |
 | status of all the coverletters which meet the users requirements to          |
 | 'DELETE'.  The coverletters are not actually deleted from the database.      |
 | This may need to be modified in the future.                                  |
 +------------------------------------------------------------------------------+

*/

switch($delete){
	case "job_all":
	  if ($confirm == "Yes"){
		if (isValidJobID_spec($job_id, $department_in_str)){
			$sql = "UPDATE coverletter 
				SET status_id = " . DELETE . " 
				WHERE job_id = " . $job_id;
		
			$result = $GLOBALS['dbh']->Execute( $sql );
			?>
			<b class="black"><?php echo $GLOBALS['dbh']->Affected_Rows()?> Cover Letter(s) Deleted</b>
			<?php
		}else{
			error("Invalid Job for Selected Department");
			include( "coverletters/admin/delete_main.inc" );
		}
	}else{
		if ($job_code == "" && $job_id == ""){
			$error = "Invalid Job Code";
		}else {
			
			$query = "SELECT specialties FROM job_info where job_code = '$job_code'";
			$select_specialties = $GLOBALS['dbh']->Execute($query);
		
			if (!($specialty_row = $select_specialties->FetchRow())) { $error = "Not available to selected department"; } else {
				$array_specialties = explode(",", $specialty_row["specialties"]);
				
				// compare specialties to department_specialties
				$query = "SELECT DISTINCT specialty_id from department_specialties where department_id IN ($department_in_str)";
				$check_depspec = $GLOBALS['dbh']->Execute($query);
				$found = 0;
				while (!$found AND $checkrow = $check_depspec->FetchRow()) {
					$i = 0;
					while($i < sizeof($array_specialties) ) {
						if ($checkrow["specialty_id"] == $array_specialties[$i]) {
							$found = 1;
						}
						$i++;
					}
				}				
				
				if ($found) {
					if ($job_id == "") {
						$sql = "SELECT i.job_id as job_id, i.job_code as job_code, e.company_name as company 
							FROM job_info as i, employer_company as e 
							WHERE i.employer_id = e.employer_id AND i.job_code = '" . addslashes(stripslashes($job_code)). "'";
					} else {
						$sql = "SELECT i.job_id as job_id, i.job_code as job_code, e.company_name as company 
							FROM job_info as i, employer_company as e 
							WHERE i.employer_id = e.employer_id and i.job_id = '$job_id'";
					}
					
					$result = $GLOBALS['dbh']->Execute($sql);
					$num_rows = $result->RecordCount();
				
				} else {
					$error = "Job Not Found In Department.";
				}
			}
		}

				//old sql		$sql = "SELECT i.job_id as job_id, i.job_code as job_code, e.company_name as company 
				//				FROM job_info as i, employer_company as e
				//				WHERE i.employer_id = e.employer_id AND i.job_code = '" 
				//					. addslashes(stripslashes($job_code)) . "'
				//					AND i.department_id IN (" . $department_in_str . ")";
			

				//				$sql = "SELECT i.job_id as job_id, i.job_code as job_code, e.company_name as company
				//			FROM job_info as i, employer_company as e
				//			WHERE i.employer_id = e.employer_id AND i.job_id = '$job_id' AND
				//				i.department_id IN (" . $department_in_str . ")";

				//		$result = $GLOBALS['dbh']->Execute( $sql );
				//		$num_rows =  $result->RecordCount();
		
		if ($error){
			error($error);
			include("coverletters/admin/delete_main.inc");
		}elseif ($num_rows == 0){
			error("Invalid Job Code");
			include( "coverletters/admin/delete_main.inc" );
		}elseif ($num_rows == 1){
			$row = $result->FetchRow();
			?>
			<table class='backgrey' cellpadding='5' width='100%'>
			  <tr>
			    <td>
			      <h4>Delete Cover Letter Confirmation</h4>
			      <p>
			      Are you sure you want to delete all cover letters for the job <?php echo $row["job_code"]?>?</p>
			      <table align="center" border="0" cellpadding="0" cellspacing="0">
			        <tr>
				  <td>
				    <table width="100%" border="0" cellpadding="0" cellspacing="0">
				      <tr>
				        <td>
						<form action="<?php echo "$PHP_SELF&amp;page=Delete";?>" method="post">
						<input type="hidden" name="select" value="coverletter" />
						<input type="hidden" name="department_id" value="<?php echo $department_id?>" />
						<input type="hidden" name="job_id" value="<?php echo $row["job_id"]?>" />
						<input type="hidden" name="job_code" value="<?php echo $job_code;?>" />
						<input type="hidden" name="delete" value="<?php echo $delete?>" />
						<input type="hidden" name="page" value="Delete" />
						<input type="submit" name="confirm" value="Yes" />Yes ! 
						</form>
					</td>
					<td>
						<form action="<?php echo $PHP_SELF?>" method="post">
						<input type="hidden" name="select" value="coverletter" />
						<input type="hidden" name="page" value="delete" />
						<input type="submit" name="confirm" value="No" />
						</form>
					</td>
				      </tr>
				    </table>
						
				  </td>
				</tr>
			     </table>
			    </td>
			  </tr>
			</table>
			<?php
			$sql = "SELECT i.job_code, e.company_name, d.description, CONCAT(s.first_name, ' ',s.last_name) as name
				FROM job_info as i, employer_company as e, student as s, coverletter as c LEFT JOIN coverletter_status as d ON d.status_id = c.status_id
				WHERE i.job_id = " . $row["job_id"] . " AND i.employer_id = e.employer_id AND
					s.student_number = c.student_number and i.job_id = c.job_id";
			$result = $GLOBALS['dbh']->Execute($sql);
			?>
			<br />
			The following cover letters exist for this job:<br />
			<table border="1">
			  <tr>
			    <td class="rowgrey"><b class="white">Student</b></td>
			    <td class="rowgrey"><b class="white">Job Code</b></td>
			    <td class="rowgrey"><b class="white">Company</b></td>
			    <td class="rowgrey"><b class="white">Status</b></td>
			  </tr>
			  <?php
			  $i = 0;
			  while($row = $result->FetchRow()){
				  (($i % 2 == 0) ? $class="row0d" : $class="row1d");
				  $i++;
				  ?>
			  	<tr>
			     	  <td class="<?php echo $class?>"><?php echo (($row["name"]=="") ? "&nbsp" : $row["name"])?></td>
			     	  <td class="<?php echo $class?>"><?php echo (($row["job_code"]=="") ? "&nbsp" : $row["job_code"])?></td>
			     	  <td class="<?php echo $class?>"><?php echo (($row["company_name"]=="") ? "&nbsp" : $row["company_name"])?></td>
			     	  <td class="<?php echo $class?>"><?php echo (($row["description"]=="") ? "&nbsp" : $row["description"])?></td>
			  	</tr>
				<?php
			}
			?>
			</table>
			<?php
		
		}else{
			?>
			<h4>Please Select One of the Following Jobs to Delete:</h4>
			
			<form method="post" action="<?php echo $PHP_SELF?>">

			<table border="1">
			  <tr>
			    <td class="rowgrey"><b class="white">&nbsp;</b></td>
			    <td class="rowgrey"><b class="white">Job Code</b></td>
			    <td class="rowgrey"><b class="white">Company</b></td>
			  </tr>
			  <?php
			  $i = 0;
			  while ($row =  $result->FetchRow()){
				(($i % 2 == 0) ? $class="row0d" : $class="row1d");
				$i++;
				?>
				<tr>
				  <td class="<?php echo $class?>"><input type="RADIO" class="<?php echo $class?>" name="job_id" value="<?php echo $row["job_id"]?>" /></td>
				  <td class="<?php echo $class?>"><?php echo (($row["job_code"] == "") ? "&nbsp;" : $row["job_code"])?></td>
				  <td class="<?php echo $class?>"><?php echo (($row["company"] == "") ? "&nbsp;" : $row["company"])?></td>
				</tr>
				<?php
			}
			?>
			</table>
			<input type="hidden" name="job_code" value="<?php echo $job_code;?>" />
			<input type="hidden" name="select" value="coverletter" />
			<input type="hidden" name="department_id" value="<?php echo $department_id?>" />
			<input type="hidden" name="delete" value="<?php echo $delete?>" />
			<input type="submit" value="Delete" name="page" />
			<input type="submit" value="Cancel" name="page" />
			</form>
			<?php
		}  
	  }
	  break;
	
	case "student_all":
	
	  if ($confirm == "Yes"){
		if (isValidStudentNumber($student_num, $department_in_str)){
			$sql = "UPDATE coverletter 
				SET status_id = " . DELETE . " 
				WHERE student_number= " . addslashes(stripslashes($student_num)) . " AND type_id = " . SPECIFIC;
			$result = $GLOBALS['dbh']->Execute( $sql );
			?>
			<b class="black"><?php echo $GLOBALS['dbh']->Affected_Rows()?> Cover Letter(s) Deleted</b>
			<?php
		}else{
			error("Invalid student number");
			include( "coverletters/admin/delete_main.inc" );
		}
	  }else{
		if ($student_num == ""){
			$error = "Please enter a Student Number";
		}else{
			$sql = "SELECT CONCAT(first_name, ' ', last_name) as name 
			FROM student 
			WHERE student_number = '" . addslashes(stripslashes($student_num)) . "'";
		
			$result = $GLOBALS['dbh']->Execute( $sql );
		}
		if ($error){
			error($error);
			include("coverletters/admin/delete_main.inc");
		}elseif ( $result->RecordCount() <= 0){
			error ("Student number - " . $student_num . " - does not exists in the selected department.");
			include( "coverletters/admin/delete_main.inc" );
		}else{
			$row = $result->FetchRow();
			$student_name = $row["name"];
			
			$sql = "SELECT i.job_code as job_code, s.description, e.company_name as company
				FROM job_info as i, employer_company as e, coverletter as c LEFT JOIN  coverletter_status as s ON s.status_id = c.status_id
				WHERE i.employer_id = e.employer_id AND 
					i.job_id = c.job_id AND 
					c.student_number = '" . addslashes(stripslashes($student_num)) . "' AND 
					c.type_id = " . SPECIFIC;
					
			$result = $GLOBALS['dbh']->Execute( $sql );
			if ( $result->RecordCount() <= 0){
				error ("No Cover Letters for Selected Student." );
				include( "coverletters/admin/delete_main.inc" );
			}else{
				?>
			<table class='backgrey' cellpadding='5' width='100%'>
			  <tr>
			    <td>
			      <h4>Delete Cover Letter Confirmation</h4>
			      <p>
			      Are you sure you want to delete all cover letters for <?php echo $student_name?>?</p>
			      <table align="center" border="0" cellpadding="0" cellspacing="0">
			        <tr>
				  <td>
				    <table align="center" border="0" width="100%" cellpadding="0" cellspacing="0">
				      <tr>
				        <td>
						<form action="<?php echo $PHP_SELF?>" method="post">
						<input type="hidden" name="select" value="coverletter" />
						<input type="hidden" name="student_num" value="<?php echo $student_num?>" />
						<input type="hidden" name="delete" value="<?php echo $delete?>" />
						<input type="hidden" name="page" value="Delete" />
						<input type="submit" name="confirm" value="Yes" />
						</form>
					</td>
					<td>
						<form action="<?php echo $PHP_SELF?>" method="post">
						<input type="hidden" name="select" value="coverletter" />
						<input type="hidden" name="student_num" value="<?php echo $student_num?>" />
						<input type="hidden" name="delete" value="<?php echo $delete?>" />
						<input type="hidden" name="page" value="delete" />
						<input type="submit" name="confirm" value="No" />
						</form>

					</td>
				      </tr>
				    </table>
				  </td>
				</tr>
			     </table>
			    </td>
			  </tr>
			</table>
			<br />The following cover letters exist for this student:<br />
				<table border="1">
				  <tr>
				    <td class="rowgrey"><b class="white">Job Code</b></td>
				    <td class="rowgrey"><b class="white">Company</b></td>
				    <td class="rowgrey"><b class="white">Status</b></td>
				  </tr>
				<?php
				$i = 0;
				while( $row =  $result->FetchRow()){
					($i % 2 == 0 ) ? $class="row0d" : $class="row1d";
					$i++;
					?>
					<tr>
					  <td class="<?php echo $class?>"><?php echo (($row["job_code"] == "") ? "&nbsp" : $row["job_code"]) ?></td>
					  <td class="<?php echo $class?>"><?php echo (($row["company"] == "") ? "&nbsp" : $row["company"]) ?></td>
					  <td class="<?php echo $class?>"><?php echo (($row["description"] == "") ? "&nbsp" : $row["description"]) ?></td>
					</tr>
				<?php
				}
				?>
				</table>
				<?php
			}	  
		}
	  }
	  break;

	case "term_all":
		if ($confirm == "Yes"){
			if ($term_id == "" || $year == ""){
				error("Must select a Term and Year.");
			}else{
			

				$sql = "SELECT job_id 
					FROM job_info 
					WHERE term_id = '" . addslashes($term_id) . "' 
						AND year = '" . addslashes($year) . "'
						 AND department_id IN (" . $department_in_str . ")";
				echo $sql;
				$result = $GLOBALS['dbh']->Execute($sql);
				$in_str = "";
				$count = 0;
				while($row = $result->FetchRow()){
					$in_str .= $row["job_id"] . ", ";
					$count ++;
				}
			
				$in_str = substr($in_str, 0, strlen($in_str) - 2);
				if ($count != 0){
					$sql = "UPDATE coverletter
						SET status_id = " . DELETE . "
						WHERE job_id IN (" . $in_str . ")";
					echo $sql;
					$result = $GLOBALS['dbh']->Execute( $sql );
					if (!$result){
						error ("An error has occured while trying to delete cover letters.");
					}else{
						$sql = "SELECT term_name
							FROM term
							WHERE term_id = '" . addslashes($term_id) . "'";
						$result = $GLOBALS['dbh']->Execute($sql);
						$row = $result->FetchRow();
						$term = $row["term_name"];
			
			echo "<p>All cover letters have to deleted for " . $term . " " . $year . ".</p>";
					}
				}else{
					error($error);
					error("There are no coverletters to delete for " . $term . " " . $year . ".");
				}
			}
			include("coverletters/admin/delete_main.inc");
		}else{
			if ($term_id == "" || $year == ""){
				error ("Must select a Term and Year.");
				include("coverletters/admin/delete_main.inc");
			}else{
				$sql = "SELECT term_name
					FROM term 
					WHERE term_id = '" . $term_id . "'";
				$result = $GLOBALS['dbh']->Execute($sql);
				$row = $result->FetchRow();
				$msg = "Are you sure you want to delete all cover letters for the " . $row["term_name"] . " " . $year . " term?";
				?>
				<table class='backgrey' cellpadding='5' width='100%'>
				  <tr>
				    <td>
				      <h4>Delete Cover Letter Confirmation</h4>
				      <p><?php echo $msg?></p>
				      <table align="center" border="0" cellpadding="0" cellspacing="0">
				        <tr>
				 	 <td>
				 	   <table align="center" border="0" width="100%" cellpadding="0" cellspacing="0">
				 	     <tr>
				 	       <td>
							<form action="<?php echo $PHP_SELF?>" method="post">
							<input type="hidden" name="select" value="coverletter" />
							<input type="hidden" name="term_id" value="<?php echo $term_id?>" />
							<input type="hidden" name="delete" value="<?php echo $delete?>" />
							<input type="hidden" name="year" value="<?php echo $year?>" />
							<input type="hidden" name="page" value="Delete" />
							<input type="submit" name="confirm" value="Yes" />
							</form>
						</td>
						<td>
							<form action="<?php echo $PHP_SELF?>" method="post">
							<input type="hidden" name="select" value="coverletter" />
							<input type="hidden" name="page" value="delete" />
							<input type="submit" name="confirm" value="No" />
							</form>
						</td>
					      </tr>
					    </table>
					  </td>
					</tr>
				     </table>
				    </td>
				  </tr>
				</table>
				<?php
			}
		}
		break;	
	case "job_student":
	  if ($confirm== "Yes"){
		  if (isValidStudentNumber($student_num, $department_in_str) && isValidJobID_spec($job_id, $department_in_str)){
			  
			$sql = "UPDATE coverletter 
				SET status_id = " . DELETE . " 
				WHERE student_number= " . addslashes(stripslashes($student_num)) . " AND 
					job_id = " . addslashes(stripslashes($job_id));
		
			$result = $GLOBALS['dbh']->Execute( $sql );
			?>
			<b class="black"><?php echo $GLOBALS['dbh']->Affected_Rows()?> Cover Letters Deleted</b>
			<?php
		}else{
			error ("Invalid Information");		
			include( "coverletters/admin/delete_main.inc" );
	  	}
	  }else{
		if ($student_num == ""){
			$error = "Invalid Student Number";
		}


		$sql = "SELECT CONCAT(first_name, ' ', last_name) as name 
			FROM student 
			WHERE student_number = '" . addslashes(stripslashes($student_num)) . "'
				AND department_id IN (" . $department_in_str . ")";
			$result = $GLOBALS['dbh']->Execute( $sql );

		
		if ($result->RecordCount() <= 0 ){
			$error = "Student number - " . $student_num . " - does not exists in the selected department.";
		}else{
			$row =  $result->FetchRow();
			$student_name  = $row["name"];
		}

		if ($job_id == "" && $job_code == ""){
			$error = "Please enter a job code.";
		}
		
		if ($error){
			error($error);
			include ( "coverletters/admin/delete_main.inc" );
		}else{
			if ($job_id == ""){
			
					$sql = "SELECT job_id, job_code, company_name as company
					FROM job_info as j, employer_company as e
					WHERE j.employer_id = e.employer_id AND 
						job_code = '" . addslashes(stripslashes($job_code)) . "' 
						";
						// AND j.department_id IN (" . $department_in_str . ")";
			
				$result = $GLOBALS['dbh']->Execute($sql);
				$num_jobs = $result->RecordCount();
				if ($num_jobs == 0){
					error("Job code does not exist in select department.");
					include("coverletters/admin/delete_main.inc");
				}elseif($num_jobs == 1){
					$row= $result->FetchRow();
					$job_id = $row["job_id"];
				}else{
					?>
					<h4>Multiple Jobs:</h4>
					Multiple Job Codes exist for job code: <?php echo $job_code?><br />
					Please select one of the following jobs:
					<form method="post" action="<?php echo $PHP_SELF?>">
	
					<table border="1">
					  <tr>
					    <td class="rowgrey"><b class="white">&nbsp;</b></td>
					    <td class="rowgrey"><b class="white">Job Code</b></td>
					    <td class="rowgrey"><b class="white">Company</b></td>
					  </tr>
					  <?php
					  $i = 0;
					  while ($row =  $result->FetchRow()){
					(($i % 2 == 0) ? $class="row0d" : $class="row1d");
						$i++;
						?>
						<tr>
						  <td class="<?php echo $class?>"><input type="RADIO" class="<?php echo $class?>" name="job_id" value="<?php echo $row["job_id"]?>" /></td>
						  <td class="<?php echo $class?>"><?php echo (($row["job_code"] == "") ? "&nbsp;" : $row["job_code"])?></td>
						  <td class="<?php echo $class?>"><?php echo (($row["company"] == "") ? "&nbsp;" : $row["company"])?></td>
						</tr>
						<?php
					}
					?>
					</table>
					<input type="hidden" name="department_id" value="<?php echo $department_id?>" />
					<input type="hidden" name="select" value="coverletter" />
					<input type="hidden" name="delete" value="<?php echo $delete?>" />
					<input type="hidden" value="<?php echo $student_num?>" name="student_num" />
					<input type="submit" value="Delete" name="page" />&nbsp;&nbsp;&nbsp;
					<input type="submit" value="Cancel" name="page" />
					</form>
					<?php
				} 
				
			}
			if ($job_id != ""){


			$query = "SELECT specialties FROM job_info where job_id = '$job_id'";
                        $select_specialties = $GLOBALS['dbh']->Execute($query);

                        if (!($specialty_row = $select_specialties->FetchRow())) { error("Not available to selected department"); include( "coverletters/admin/delete_main.inc"); } else {
                                $array_specialties = explode(",", $specialty_row["specialties"]);

                                // compare specialties to department_specialties
                                $query = "SELECT DISTINCT specialty_id from department_specialties where department_id IN ($department_in_str)";
                                $check_depspec = $GLOBALS['dbh']->Execute($query);
                                $found = 0;
                                while (!$found AND $checkrow = $check_depspec->FetchRow()) {
                                        $i = 0;
                                        while($i < sizeof($array_specialties) ) {
                                                if ($checkrow["specialty_id"] == $array_specialties[$i]) {
                                                        $found = 1;
                                                }
                                                $i++;
                                        }
                                }

                                if ($found) {
                                        
					$sql = "SELECT i.job_id as job_id, i.job_code as job_code, e.company_name as company, s.description
					FROM job_info as i, employer_company as e, coverletter as c LEFT JOIN coverletter_status as s ON s.status_id = c.status_id
					WHERE i.employer_id = e.employer_id AND i.job_id = '" . addslashes(stripslashes($job_id)) . "' 
						AND c.job_id = i.job_id AND c.student_number = '" . addslashes(stripslashes($student_num)) . "'";

					$result = $GLOBALS['dbh']->Execute($sql);
                                        $num_rows = $result->RecordCount();

                                } else {
                                        error("Job Not Found In Department.");
					include ("coverletters/admin/delete_main.inc");
                                }
                        }
		
				//	$sql = "SELECT i.job_id as job_id, i.job_code as job_code, e.company_name as company, s.description
				//	FROM job_info as i, employer_company as e, coverletter as c LEFT JOIN coverletter_status as s ON s.status_id = c.status_id
				//	WHERE i.employer_id = e.employer_id AND i.job_id = '" . addslashes(stripslashes($job_id)) . "' 
				//		AND c.job_id = i.job_id AND c.student_number = '" . addslashes(stripslashes($student_num)) . "' AND
				//		i.department_id IN (" . $department_in_str . ")";        ######################
				//$result = $GLOBALS['dbh']->Execute( $sql );

				//$num_rows =  $result->RecordCount();

				if ($num_rows == 0){
					error("No Cover Letters for Selected Job Code and Student");
					include( "coverletters/admin/delete_main.inc" );
				}elseif ($num_rows == 1){
					$row = $result->FetchRow();
					?>
					<table class='backgrey' cellpadding='5' width='100%'>
					  <tr>
					    <td>
					      <h4>Delete Cover Letter Confirmation</h4>
					      <p>
					      Are you sure you want to delete all cover letters for student <?php echo $student_name?> and job <?php echo $row["job_code"]?>?</p>
					      <table align="center" border="0" cellpadding="0" cellspacing="0">
					        <tr>
					 	 <td>
					 	   <table align="center" border="0" width="100%" cellpadding="0" cellspacing="0">
					 	     <tr>
					 	       <td>
								<form action="<?php echo $PHP_SELF?>" method="post">
								<input type="hidden" name="department_id" value="<?php echo $department_id?>" />
								<input type="hidden" name="select" value="coverletter" />
								<input type="hidden" name="student_num" value="<?php echo $student_num?>" />
								<input type="hidden" name="job_id" value="<?php echo $row["job_id"]?>" />
								<input type="hidden" name="delete" value="<?php echo $delete?>" />
								<input type="hidden" name="page" value="Delete" />
								<input type="submit" name="confirm" value="Yes" />
								</form>
							</td>
							<td>
								<form action="<?php echo $PHP_SELF?>" method="post">
								<input type="hidden" name="select" value="coverletter" />
								<input type="hidden" name="delete" value="<?php echo $delete?>" />
								<input type="hidden" name="page" value="delete" />
								<input type="submit" name="confirm" value="No" />
								</form>

							</td>
						      </tr>
						    </table>
						  </td>
						</tr>
					     </table>
					    </td>
					  </tr>
					</table>
					<br />
					The following cover letters exist for this student and job:
					<table border="1">
					  <tr>
					    <td class="rowgrey"><b class="white">Job Code</b></td>
					    <td class="rowgrey"><b class="white">Company</b></td>
					    <td class="rowgrey"><b class="white">Status</b></td>
					  </tr>
					  <tr>
					    <td class="row0d"><?php echo (($row["job_code"]=="") ? "&nbsp" : $row["job_code"])?></td>
					    <td class="row0d"><?php echo (($row["company"]=="") ? "&nbsp" : $row["company"])?></td>
					    <td class="row0d"><?php echo (($row["description"]=="") ? "&nbsp" : $row["description"])?></td>
					  </tr>
					</table>
					<?php
				}	
			}
  		  }
		}
		break;	

	  default:
	  	error ("Please select a delete option");
		include( "coverletters/admin/delete_main.inc" );
}


?>
