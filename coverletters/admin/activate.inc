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
 | Filename: activate.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description: Once the user has selected a job this page displays all the     |
 | students with coverletters for the job and allows the user to select the     |
 | coverletters they wish to activate.  Once select the status for the          |
 | coverletter is updated to ACTIVE. (Once the coverletters are updated a form  |
 | to employer the employer the information is displayed).                      |
 +------------------------------------------------------------------------------+

*/


if ($job_code == "" && $job_id == ""){
	error( "Invalid Job Code" );
	include ( "coverletters/admin/activate_main.inc" );
}else{
	if ($sure == "yes"){
		if (!is_array($student) || (is_array($student) && sizeof($student) <= 0)){
			error( "No Students Selected" );
			$sure = "";
		}elseif ($job_id == "" || !isValidJobID_spec($job_id, $department_in_str)){
			error ("Invalid Job ID");
			$sure = "";
		}else{
			foreach ($student as $s){
				$in_str .= "'" . $s . "',";
			}
			$in_str = substr($in_str, 0, strlen($in_str) - 1);
			$sql = "UPDATE coverletter SET status_id = '" . ACTIVE . "' 
				WHERE student_number IN (" . $in_str . ") AND job_id = $job_id AND status_id IS NULL";
			$result = $GLOBALS['dbh']->Execute($sql);
			include("coverletters/admin/email_employer.inc");
		}
	}elseif ($sure != "yes"){
		if ($job_id == ""){

			/* old sql statement (verifies only with department).
				$sql = "SELECT i.job_code as job_code, i.job_id as job_id, e.company_name as company
				FROM job_info as i, employer_company as e, term as t
				WHERE i.job_code = '" . addslashes(stripslashes($job_code)) . "' AND i.employer_id = e.employer_id
					AND i.department_id In (" . $department_in_str . ") AND t.term_id = i.term_id
					AND i.year = YEAR(CURRENT_DATE()) AND MONTH(CURRENT_DATE()) BETWEEN t.start_date AND t.end_date";

			/* new method.. compares specialties */
                	// Find Specialties applicable to this job_code
	                $select_specialties = $GLOBALS['dbh']->Execute("SELECT specialties FROM job_info where job_code = '$job_code'");
        	        if ($specialty_row = $select_specialties->FetchRow()) {
                	        $array_specialties = explode(",", $specialty_row["specialties"]);

                        	// Compare specialities to the department_specialties

	                        $query = "SELECT specialty_id from department_specialties where department_id IN ($department_in_str)";
	                        $check_depspec = $GLOBALS['dbh']->Execute($query);

	                        $found = 0;
        	                if ($department_id == 0) {  // Search All
                	                $found = 1;
                       		 }

	                        while (!$found AND $checkrow = $check_depspec->FetchRow()) {
        	                        $i = 0;
                	                while ($i < sizeof($array_specialties) ) {
                        	                if ($checkrow["specialty_id"] == $array_specialties[$i]) {
                                	                $found = 1;
                                        	}
	                                        $i++;
        	                        }
                	        }

                        	if ($found) {
	                                $show = 1;

        	                        $sql = "SELECT i.job_id as job_id, e.company_name as company, i.job_code as job_code
                	                        FROM job_info as i left join employer_company as e ON e.employer_id = i.employer_id
                        	                WHERE job_code = '" . addslashes(stripslashes($job_code)) . "'";
                                	$result = $GLOBALS['dbh']->Execute( $sql );

	                                $numrows =  $result->RecordCount();


				if ($num_rows <= 0){
					error("Invalid job code for current term.");
					include("coverletters/admin/activate_main.inc");
				}elseif ($num_rows == 1){
					$row =  $result->FetchRow();
					$job_id = $row["job_id"];
				}else{
					?>
					<h4>Please select which of the following jobs you wish to activate:</h4>
					<form method="post" action="<?php echo $PHP_SELF?>">
					<input type="hidden" name="select" value="coverletter" />
					<table border="1">
					  <tr>
					    <td class="rowgrey"><b class="white">&nbsp;</b></td>
					    <td class="rowgrey"><b class="white">Job Code</b></td>
					    <td class="rowgrey"><b class="white">Company</b></td>
					  </tr>
					  <?php
					  $i = 0;
					  while($row =  $result->FetchRow()){
						 ($i % 2 == 0) ? $class = "row0d" : $class="row1d";
						 $i ++;
						 ?>
						 <tr>
						   <td class="<?php echo $class?>"><input type="radio" class="<?php echo $class?>" name="job_id" value="<?php echo $row["job_id"]?>" /></td>
						   <td class="<?php echo $class?>"><?php echo (($row["job_code"] == "") ? "&nbsp;" : $row["job_code"])?></td>
						   <td class="<?php echo $class?>"><?php echo (($row["company"] == "") ? "&nbsp;" : $row["company"])?></td>
						 </tr>
					<?php
					  }
					  ?>
					</table>
					<input type="submit" value="Activate" name="page" />
					</form>
				<?php
				}
			} else { 
					error("Invalid job code for current term or department.");
					include("coverletters/admin/activate_main.inc");
			}
		} else {
					error("Invalid job code for current term department.");
					include("coverletters/admin/activate_main.inc");
		}
	}
	}
	if ($job_id != "" && $sure != "yes"){
		$sql = "SELECT job_code 
			FROM job_info 
			WHERE job_id=$job_id";
		$result = $GLOBALS['dbh']->Execute( $sql );
		$row =  $result->FetchRow();
		$job_code = $row["job_code"];
		
		
		$sql = "SELECT CONCAT(s.first_name, ' ', s.last_name) as name, c.status_id,  
				c.student_number as student_number
			FROM coverletter as c, student as s
			WHERE c.student_number = s.student_number AND c.job_id = " . addslashes(stripslashes($job_id));

		$result = $GLOBALS['dbh']->Execute( $sql );

		$numrows =  $result->RecordCount();

		if ($numrows == 0){
			?>
			<h3>Activate</h3>
			<b class="black">No cover letters exist for job <?php echo $job_code?>.</b>
			<?php
		}else{
			while($row =  $result->FetchRow()){
				if ($row["status_id"] == ACTIVE){
					$active_name[] = $row["name"];
					$active_student[] = $row["student_number"];
				}elseif ($row["status_id"] == ""){
					$apply_name[] = $row["name"];
					$apply_student[] = $row["student_number"];
				}
			}
			
			?>
			
			<h3>Activate</h3>
			<b class="black">Job Code: </b><?php echo $job_code?>
			<form method="post" ACTIVATE="<?php echo $PHP_SELF?>">
			<input type="hidden" name="select" value="coverletter" />
			<input type="hidden" name="sure" value="yes" />
			<input type="hidden" name="job_id" value="<?php echo $job_id?>" />
			<?php
			if (sizeof($apply_student) <= 0 && sizeof($active_student) <= 0){
				?>
				<b class="black">No students have applied to this job.</b><br />
				<?php
			}elseif (sizeof($active_student) > 0 && sizeof($apply_name) <= 0){
				?>
				<b class="black">All students applications have been activated.</b><br />
				<?php
			}
			if (sizeof($apply_student) > 0){
				?>
				<input type="submit" name="page" value="Activate" /><br />
				<br /><p>Please select which cover letters you wish to activate:</p>
				<table border="1">
				  <tr>
		 		    <td class="rowgrey"><b class="white">&nbsp;</td>
				    <td class="rowgrey"><b class="white">Student Number</td>
				    <td class="rowgrey"><b class="white">Student Name</td>
				  </tr>
				  <?php
			  	 for($i = 0;$i<sizeof($apply_student);$i++){
				  	($i % 2 == 0) ? $class = "row0d" : $class = "row1d";
				  
				  	$href = $PHP_SELF . "&amp;select=coverletter&amp;page=viewapplication&amp;PDF=1&amp;student=" . $apply_student[$i] . "&job_id=" . $job_id;
					  ?>
					  <tr>
					    <td class="<?php echo $class?>"><input type="checkbox" value="<?php echo $apply_student[$i]?>" name="student[]" checked='checked' /></td>
					    <td class="<?php echo $class?>"><?php echo (($apply_student[$i] == "") ? "&nbsp;" : $apply_student[$i])?></td>
					    <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($apply_name[$i] == "") ? "&nbsp;" : $apply_name[$i])?></a></td>
					  </td>
					<?php
				  }
			  	?>
				</table>
				<br /><input type="submit" value="Search Applicants" name="page"i />

			<?php
			}
			if (sizeof($active_student) > 0){
				?>
				<br /><br /><br /><p>The following cover letters have been activated for this job:</p>
				<table border="1">
				  <tr>
				    <td class="rowgrey"><b class="white">Student Number</td>
				    <td class="rowgrey"><b class="white">Student Name</td>
				  </tr>
				  <?php
			  	 for($i = 0;$i<sizeof($active_student);$i++){
				  	($i % 2 == 0) ? $class = "row0d" : $class = "row1d";
					  ?>
					  <tr>
					    <td class="<?php echo $class?>"><?php echo (($active_student[$i] == "") ? "&nbsp;" : $active_student[$i])?></td>
					    <td class="<?php echo $class?>"><?php echo (($active_name[$i] == "") ? "&nbsp;" : $active_name[$i])?></td>
					  </td>
					<?php
				  }
			  	?>
				</table>
			<?php
			}
			?>
			</form>
			<?php
		}
	}
}
?>
