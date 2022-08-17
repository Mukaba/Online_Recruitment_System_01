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
 | Filename: add_letter.inc                                                     |
 +------------------------------------------------------------------------------+
 | Description: Sets up the information needed by edit.inc to edit the          |
 | coverletter for the job.  Finds the job id corresponding to the job code     |
 | the user entered - if multiple job codes that are the same then prompts the  |
 | user to select one.                                                          |
 +------------------------------------------------------------------------------+

*/

if (!isset ($job_code) && isset ($job_id))
{
	// job code not always set, we can pull it out.
	global $job_code;

	$sql = "select job_code from job_info where job_id=\"$job_id\"";
	$response = $GLOBALS['dbh']->Execute ($sql);

	assert ($response->RecordCount() == 1);
	$row = $response->FetchRow();

	$job_code = $row["job_code"];
}

if ($page == "Add Template"){
	$job_id = "0";
	$type_id = TEMPLATE;
	$job_code = "Template";
	include ("coverletters/student/edit.inc");
}elseif($page == "Add Generic"){
	$job_id = "0";
	$type_id = GENERIC;
	$job_code = "Generic";
	include ("coverletters/student/edit.inc");
}elseif ($page == "Add Job"){
	if ($job_id == "" && trim($job_code) == ""){
		error( "Invalid Job Code" );
		include( "coverletters/student/add.inc" );
	}else{

	// new code, verifies specialties...
	// removed (no longer used)

                // Find Specialties applicable to this job_code
                //$select_specialties = $GLOBALS['dbh']->Execute("SELECT specialties FROM job_info where job_code = '$job_code'");
	/*
								$sql = "select specialties from job_info where job_code=\"$job_code\"";
								error ($sql);
								$select_specialties = $GLOBALS['dbh']->Execute ($sql);
								error ($select_specialties->RecordCount());
                if ($specialty_row = $select_specialties->FetchRow()) {
												error ("looks OK");
                        $array_specialties = explode(",", $specialty_row["specialties"]);

                        // Compare specialities to the department_specialties
                        $query = "SELECT specialty_id from department_specialties where department_id = $auth->department";
                        $check_depspec = $GLOBALS['dbh']->Execute($query);

                        $found = 0;

                        while (!$found AND $checkrow = $check_depspec->FetchRow()) {
                                $i = 0;
                                while ($i < sizeof($array_specialties) ) {  // could use inarray function
                                        if ($checkrow["specialty_id"] == $array_specialties[$i]) {
                                                $found = 1;
                                        }
                                        $i++;
                                }
                        }
*/

 //                       if ($found) {
                                $show = 1;
				

                                $sql = "SELECT i.job_id as job_id, e.company_name as company, i.job_code as job_code
                                        FROM job_info as i left join employer_company as e ON e.employer_id = i.employer_id 
                                        WHERE job_code = '" . addslashes(stripslashes($job_code)) . "'".(isset($job_id) ? " and job_id=\"$job_id\"" : "");
				 $result = $GLOBALS['dbh']->Execute( $sql );

				$rows = $result->RecordCount();

				if ($rows == 0){
					//job code not in the database
					error ( "Invalid Job Code or Job not in department." );
					include ("coverletters/student/add.inc");
				}elseif ($rows == 1){
					//only one job code in database
					$row =  $result->FetchRow();
					$job_id = $row["job_id"];
					$type_id = SPECIFIC;
			
					if (check_existance($job_id, $type_id, $student_number)){
						error("Cover letter already exists for job code: <b class=\"black\">" . $row["job_code"] . "</b>.  Please use the edit menu to edit the cover letter.");
						include( "coverletters/student/add.inc" );
					}else{
						if ($use_generic == "on"){
							include ("coverletters/save.inc");
							include ("coverletters/student/view.inc");
						}else{
							include ("coverletters/student/edit.inc");
						}
					}
		
				}else{
					//more than one job code in the database - student needs to select the one they want
	
					while($row =  $result->FetchRow()){
						if (!(check_existance($row["job_id"], SPECIFIC, $student_number))){
							$HREF = $PHP_SELF . "&amp;select=coverletter&amp;page=Add+Job&use_generic=" . urlencode($use_generic) . "&amp;suppress_transcript=" . urlencode($suppress_transcript) . "&amp;job_id=" . urlencode($row["job_id"]) . "&amp;type_id=" . urlencode(SPECIFIC)."&amp;resume_id=".$resume_id;
							$urls[] = $HREF;
							$codes[] = $row["job_code"];
							$companies[] = $row["company"];
						}else{
							$codes_applied[] = $row["job_code"];
							$companies_applied[] = $row["company"];
						}
		
					}
	
					if (sizeof($urls) <= 0 ){
						error("Cover letters already exist for all jobs job code " . $job_code . ".  Please use the edit menu to edit these jobs");
						include ( "coverletters/student/add.inc" );
					}else{
	
				?>
					<table width="100%" cellpadding="5" cellspacing="0" border="0">
					  <tr>
					    <td><h4>Select which of the following jobs you wish to apply for:</h4></td>
					  </tr>
					  <tr>
					    <td>
					      <table border="1">
					        <tr>
						  <td class="rowgrey"><b class="white">Job Code</b></td>
						  <td class="rowgrey"><b class="white">Employer</b></td>
						</tr>
					      <?php
					      for($i=0;$i<sizeof($urls);$i++){
						  ($i % 2) ? $class = "row0d" : $class = "row1d";
						  ?>
						  <tr>
						    <td class="<?php echo $class?>"><a class="blue" HREF ="<?php echo $urls[$i]?>"><?php echo $codes[$i]?></a></td>
					  	    <td class="<?php echo $class?>"><a class="blue" HREF ="<?php echo $urls[$i]?>"><?php echo (($companies[$i]=="")?"&nbsp;" : $companies[$i])?></a></td>
					          </tr>
					      <?php
					      }
					      ?>
					      </table>
					    </td>
					  </tr>
					<?php
					if (sizeof($codes_applied) > 0){
					?>
					  <tr>
					    <td><br /><hr width="100%" /><h5>Cover letters exist for the following jobs</h5></td>
					  </tr>
					  <tr>
					    <td>
					      <table border="1">
					        <tr>
						  <td class="rowgrey"><b class="white">Job Code</b></td>
						  <td class="rowgrey"><b class="white">Employer</b></td>
						</tr>
						<?php
						for($i = 0; $i<sizeof($codes_applied); $i++){
							($i % 2) ? $class="row0d" : $class="row1d";	
							?>
							<tr>
							  <td class="<?php echo $class?>"><?php echo $codes_applied[$i]?></td>
							  <td class="<?php echo $class?>"><?php echo $companies_applied[$i]?></td>
							</tr>
							<?php
						}
						?>
					     </table>
					   </td>
					 </tr>
		       		 <?php
					}
					?>
					</table>
							
				<?php
					}
				}
		/*
		} else {
					include ("coverletters/student/add.inc");
		}
	
} else { 
					error ( "Invalid Job Code or Job not in department." );
					include ("coverletters/student/add.inc");
}
*/	
}
}	
/*******************************************************************************************/
/* check is the student already has a coverletter for job: job_id and type: type_id in the database.
*  returns true if exists, false otherwise
*/
function check_existance($job_id, $type_id, $student_number){
	$sql = "SELECT job_id 
		FROM coverletter 
		WHERE student_number = '$student_number' AND type_id = " . addslashes(stripslashes($type_id)) . " AND job_id = " . addslashes(stripslashes($job_id));
	$result = $GLOBALS['dbh']->Execute( $sql );
	$rows =  $result->RecordCount();
	if ($rows > 0 ){
		return true;
	}else{
		return false;
	}
}
?>
