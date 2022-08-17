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
 | Filename: view_applications.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: Allows admin to view applications in the same way an employer   |
 | would.  This page lists all the students whose coverletters are active for   |
 | the select job and allows the user to view some or all of the applicants in  |
 | PDF format.                                                                  |
 +------------------------------------------------------------------------------+

*/


if ($page == "View All Applications"){
	error ("Unknown Error");
}elseif($page == "View Selected Applications"){
	error ("Please select which applications you wish to view");
}

if ($job_code == "" && $job_id == ""){
	error( "Invalid Job Code" );
	include ("coverletters/admin/view_applications_main.inc");

}else{
	if ($job_id == ""){
			
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
                                                FROM job_info as i, employer_company as e
                                                WHERE job_code = '" . addslashes(stripslashes($job_code)) . "' AND i.employer_id = e.employer_id";
                                        $result = $GLOBALS['dbh']->Execute( $sql );

                                        $numrows =  $result->RecordCount();


				if ($num_rows <= 0){
					error("Job code does not exist in the selected department for the current term.");
					include ("coverletters/admin/view_applications_main.inc");
				}elseif ($num_rows == 1){
					$row =  $result->FetchRow();
					$job_id = $row["job_id"];
			}else{
				error("Multiple jobs exist with Job Code - ".$job_code);
				instruct("Please select which of the following jobs you wish to view:");
				?>
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
					   <td class="<?php echo $class?>"><input class="<?php echo $class?>" type="RADIO" name="job_id" value="<?php echo $row["job_id"]?>" /></td>
					   <td class="<?php echo $class?>"><?php echo (($row["job_code"] == "") ? "&nbsp;" : $row["job_code"])?></td>
					   <td class="<?php echo $class?>"><?php echo (($row["company"] == "") ? "&nbsp;" : $row["company"])?></td>
					 </tr>
					<?php
				  }
				  ?>
				</table>
				<input type="submit" value="View Applications" name="page" />
				</form>
			<?php
			}
		} else {
			error("Job code does not exists in the selected department for the current term.");
			include ("coverletters/admin/view_applications_main.inc");
		
		}
	} else {
		error("Job code does not exists in the selected department for the current term.");
		include ("coverletters/admin/view_applications_main.inc");

	}

	}
	if ($job_id != ""){
		$sql = "SELECT job_code 
			FROM job_info 
			WHERE job_id = " . addslashes(stripslashes($job_id));
		$result = $GLOBALS['dbh']->Execute( $sql );
		$row =  $result->FetchRow();
		$job_code = $row["job_code"];
		
		
		$sql = "SELECT CONCAT(s.first_name, ' ', s.last_name) as name, c.student_number as student_number
			FROM coverletter as c, student as s
			WHERE c.student_number = s.student_number AND c.job_id = " . addslashes(stripslashes($job_id)) . " 
				AND c.status_id= " . ACTIVE;


		$result = $GLOBALS['dbh']->Execute( $sql );

		$numrows =  $result->RecordCount();

		if ($numrows == 0){
			?>
			<h3>View Applications</h3>
			<b class="black">No active cover letters exist for job: <?php echo $job_code?></b>
			<?php
		}else{
			?>
			<h3>View Applications</h3>
			<b class="black">Job Code: </b><?php echo $job_code?>
			<br />The following active cover letters exist for this job:<br /><br />
			<form method="post" ACTIVATE="<?php echo $PHP_SELF?>">
			<input type="hidden" name="select" value="coverletter" />
			<input type="hidden" name="job_id" value="<?php echo $job_id?>" />
			<input type="hidden" name="PDF" value="1" />
			<input type="submit" name="page" value="View All Applications" />
			<table border="1">
			  <tr>
			    <td class="rowgrey"><b class="white">&nbsp;</td>
			    <td class="rowgrey"><b class="white">Student Number</td>
			    <td class="rowgrey"><b class="white">Student Name</td>
			  </tr>
			  <?php
			  $i = 0;
			  while($row =  $result->FetchRow()){
				  ($i % 2 == 0) ? $class = "row0d" : $class = "row1d";
				  $i ++;
				  $href = $PHP_SELF .  "&amp;select=coverletter&amp;page=viewapplication&amp;PDF=1&amp;job_id=" . urlencode($job_id) . "&amp;student=" . urlencode($row["student_number"]);
				  ?>
				  <tr>
				    <td class="<?php echo $class?>"><input class="<?php echo $class?>" type="checkbox" name="student[]" value="<?php echo $row["student_number"]?>" /></td>
				    <td class="<?php echo $class?>"><?php echo (($row["student_number"] == "") ? "&nbsp;" : $row["student_number"])?></td>
				    <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($row["name"] == "") ? "&nbsp;" : $row["name"])?></a></td>
				  </td>
			<?php
			  }
			  ?>
			</table>
			<input type="submit" name="page" value="View Selected Applications" />
			</form>
			<?php
		}
	}
}
?>
