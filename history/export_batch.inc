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
 | Filename: export_batch.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: Allows individual to print out all view history records from    |
 | advanced search. Derived from view_history.inc   :TODO: Clean up code        |
 +------------------------------------------------------------------------------+

*/
?>
		</td><td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt="" /></td></tr></table>
<?php
    //Show Sub menus
    echo("<h3>Student Information - Placement History</h3>");
    include_once('history/functions.inc');
    include_once('history/container_functions.inc');
    $arrHistoryID = array();
    $history_id_list = urldecode($history_id_list);
    $history_id_list = trim($history_id_list);
    $arrHistoryID = explode(",", $history_id_list);
    $pdfOutput;

    foreach ($arrHistoryID as $history_id)
    {
        // In a foreach loop, variables are not scoped locally for each iteration. Therefore, we have to unset variables every iteration. 
        unset($historyOutput);
		unset($current_address_output);
        unset($permanent_address_output);
        unset($address_output);
        unset($flags_output);
        unset($history_row);
        unset($report_subject_row);
        unset($wt_status_row);
        unset($options_row);
        unset($contact_row);
        unset($site_visit_type_row); 
        unset($supervisor_site_visit_type_row);
        unset($supervisor_visitor_row);
        unset($record_row);
        unset($student_row);
        unset($employer_row);
        unset($contact_row);
        unset($contact_department_row);
        unset($visitor_row);
        unset($province_row);
        unset($country_row);
        unset($contact_id_row);
		unset($company_street_address_1);
		unset($company_street_address_2);
        unset($company_department_row);
        unset($province);
        unset($company_country);
        unset($company_province);
        unset($wt_code_row);
        unset($wt_codes);

		$department_in_str=implode("', '",$departments_in_group);
		//get the history information from all the different tables
		$history_sql=("
			SELECT h.options, h.wt_status, h.company_name, h.department_name, h.supervisor_name, h.year, h.supervisor_position
            , h.supervisor_email, h.supervisor_phone_number, h.supervisor_cellphone_number, h.supervisor_fax_number
            , h.work_term_street_address_1, h.work_term_street_address_2
            , h.work_term_country, h.work_term_province, h.work_term_city, h.company_city, h.company_country, h.company_province, h.company_postal_code
            , h.company_street_address_1, h.company_street_address_2, h.site_visit_type_id_supervisor, h.work_term_home_phone, h.site_visit_by_supervisor
            , h.site_visit_date_supervisor, h.site_visit_type_id, h.work_site_visit_notes, h.student_number, h.department_id, h.comment, h.report_title
            , h.report_subject, h.report_marker, h.report_code, h.site_visit_date, h.site_visit_by, h.student_work_phone, h.employer_id, h.contact_id
            , h.job_title, h.term_id, h.work_term_length, h.work_email, h.salary, h.company_region_id, ji.job_code, sp.salary_period_name, h.work_term_hours_per_week
            , h.start_date, h.end_date, h.work_term_number
			FROM history h
            LEFT JOIN job_info ji
            ON h.job_id = ji.job_id
            LEFT JOIN salary_period sp
            ON h.salary_period_id = sp.salary_period_id
            LEFT JOIN history_container hc
            ON h.history_id = hc.history_id
			WHERE h.history_id='".$history_id."'
			");

		$history_results=$GLOBALS['dbh']->Execute($history_sql);
		$history_row=$history_results->FetchRow();
		$history_row['term_name']=getTermName($history_row['term_id']);

        $wt_code_sql = ("
            SELECT workterm_code
            FROM history_container
            WHERE history_id = '".$history_id."'
            ORDER BY workterm_code
            ");
        $wt_code_result = $GLOBALS['dbh']->Execute($wt_code_sql);
        // create comma delimited list of workterm codes if the student is using container based history records. 
        if ($wt_code_result->RecordCount())
        {
            while ($wt_code_row = $wt_code_result->FetchRow())
            {
                $wt_codes[] = $wt_code_row['workterm_code'];
            }
            $wt_codes = implode(",",$wt_codes);
            $history_row['work_term_number'] = $wt_codes;
        } 
        
		if ($history_row['report_subject']<>NULL){
			$report_subject_sql= "
				SELECT report_subject_name 
				FROM history_report_subject 
				WHERE report_subject_id='".$history_row['report_subject']."'
				";
			$report_subject_results=$GLOBALS['dbh']->Execute($report_subject_sql);
			$report_subject_row=$report_subject_results->FetchRow();
		}
		if ($history_row['wt_status']<>NULL){
			$wt_status_sql = "SELECT history_status_name 
					  FROM history_status 
					  WHERE history_status_id=".$history_row['wt_status'];
			$wt_status_results=$GLOBALS['dbh']->Execute($wt_status_sql);
			$wt_status_row=$wt_status_results->FetchRow();
		}
		if ($history_row['options']<>NULL){
			$options_sql = "SELECT history_options_name 
					FROM history_options 
					WHERE history_options_id=".$history_row['options'];
			$options_results = $GLOBALS['dbh']->Execute($options_sql); 
			$options_row = $options_results->FetchRow();   
		}
		if ($history_row['contact_id']<>NULL){
			$contact_sql="Select first_name,last_name,phone,cellphone,fax from contact where contact_id=".$history_row['contact_id'];
			$contact_results=$GLOBALS['dbh']->Execute($contact_sql);
			$contact_row=$contact_results->FetchRow(); 		
		}
		if ($history_row['site_visit_type_id']<>NULL){
			$site_visit_type_sql = "Select description from site_visit_type where site_visit_type_id=".$history_row['site_visit_type_id']; 
			$site_visit_type_results = $GLOBALS['dbh']->Execute($site_visit_type_sql);
			$site_visit_type_row = $site_visit_type_results->FetchRow();
		}
		if ($history_row['site_visit_type_id_supervisor']<>NULL){
			$site_visit_type_sql = "Select description from site_visit_type where site_visit_type_id=".$history_row['site_visit_type_id_supervisor'];
			$site_visit_type_results = $GLOBALS['dbh']->Execute($site_visit_type_sql);
			$supervisor_site_visit_type_row = $site_visit_type_results->FetchRow();
		}
		$sql = "SELECT record_id,department_id 
			FROM student_department 
			WHERE student_number=".$history_row['student_number']." AND department_id IN ('".$department_in_str."')";
		$results = $GLOBALS['dbh']->Execute($sql);
		if ($results->RecordCount()>1){
			while($record_row=$results->FetchRow()){
				if ($record_row['department_id']==$auth->department){
					$record_id=$record_row['record_id'];
				}
			}
			if ($record_id==NULL){
				$record_id=$record_row['record_id'];
			}
		}else{
			$record_row=$results->FetchRow(); 
			$record_id=$record_row['record_id'];
		}
		if ($history_row['student_number']<>NULL){
			
			$student_sql="SELECT s.first_name,s.photo,sd.department_id,s.last_name,s.email,s.gender,d.discipline_name
                          FROM student as s
                          INNER JOIN student_department as sd 
                          ON s.student_number=sd.student_number
                          LEFT JOIN discipline d
                          ON sd.discipline_id = d.discipline_id
                          WHERE sd.record_id='".$record_id."'";
			$student_results=$GLOBALS['dbh']->Execute($student_sql);

			if ($student_results->RecordCount()>0){
				$student_sql=NULL;
				while ($student_row=$student_results->FetchRow()){
					if ($student_row['department_id']==$auth->department){
						$student_sql="SELECT s.first_name, s.student_number, s.photo, sd.department_id, s.last_name, s.email, s.gender, d.discipline_name
                                      FROM student as s
                                      INNER JOIN student_department as sd
                                      ON s.student_number=sd.student_number
                                      LEFT JOIN discipline d
                                      ON sd.discipline_id = d.discipline_id
                                      WHERE s.student_number=".$history_row['student_number']." AND sd.department_id=".$student_row['department_id'];
					}
				}
				if ($student_sql<>NULL){
					$student_results=$GLOBALS['dbh']->Execute($student_sql);
					$student_row=$student_results->FetchRow();
				}else{
					$student_sql="SELECT s.first_name, s.student_number, s.photo, sd.department_id, s.last_name, s.email, s.gender, d.discipline_name
                                  FROM student as s
                                  INNER JOIN student_department as sd 
                                  ON s.student_number=sd.student_number
                                  LEFT JOIN discipline d
                                  ON sd.discipline_id = d.discipline_id
                                  WHERE s.student_number=".$history_row['student_number']." AND sd.department_id IN ('".$department_in_str."')";
					$student_results=$GLOBALS['dbh']->Execute($student_sql);
					$student_row=$student_results->FetchRow();
				}
			}
		}

		if ($history_row['employer_id']<>NULL){
			$employer_sql="SELECT company_name,street_address_1,street_address_2,city,provstate_id,country_id,postal_code 
				       FROM employer_company 
				       WHERE employer_id=".$history_row['employer_id'];
			$employer_results=$GLOBALS['dbh']->Execute($employer_sql);
			$employer_row=$employer_results->FetchRow();
		}

		if ($history_row['site_visit_by']<>NULL){
			$visitor_sql="Select first_name,last_name from contact where contact_id=".$history_row['site_visit_by'];
			$visitor_results=$GLOBALS['dbh']->Execute($visitor_sql);
			$visitor_row=$visitor_results->FetchRow();
		}

		if ($history_row['site_visit_by_supervisor']<>NULL){
			$visitor_sql="Select first_name,last_name from contact where contact_id=".$history_row['site_visit_by_supervisor'];
			$visitor_results=$GLOBALS['dbh']->Execute($visitor_sql);
			$supervisor_visitor_row=$visitor_results->FetchRow();
		}

		$sql = "Select notes, contact_id, date_entered from history_notes where history_id=".$history_id." order by date_entered desc";
		$notesresults=$GLOBALS['dbh']->Execute($sql);
		$company_street_address_1=$history_row['company_street_address_1'];
		$company_street_address_2=$employer_row['company_street_address_2'];
		$company_city=$history_row['company_city'];

		if ($history_row['company_country']<>NULL){
			$company_country=getCountryName($history_row['company_country']);
		}

		if ($history_row['company_province']<>NULL){
			$company_province=getProvStateName($history_row['company_province']);
		}

		$company_postal_code=$history_row['company_postal_code'];

		if ($history_row['company_name']==NULL){
			$history_row['company_name']=$employer_row['company_name'];
		}

		$work_term_street_address_1 = $history_row['work_term_street_address_1'];
		$work_term_street_address_2 = $history_row['work_term_street_address_2'];
		$work_term_city=$history_row['work_term_city'];
		$work_term_province = getProvStateName($history_row['work_term_province']);
		$work_term_country = getCountryName($history_row['work_term_country']);

		if ($contact_row['department_id']<>NULL){
			$company_department_sql="SELECT department_name, street_address_1,street_address_2, city, provstate_id,country_id,postal_code 
						 FROM employer_department 
						 WHERE employer_id=".$history_row['company_department_id']; 
			$company_department_results=$GLOBALS['dbh']->Execute($comapny_department_sql);
			$company_department_row=$company_department_results->FetchRow();
			
			$company_street_address_1=$company_department_row['street_address_1'];
			$company_street_address_2=$company_department_row['street_address_2'];
			$company_city=$company_department_row['city'];
			$company_country=$company_department_row['country_id'];
			$company_province=$company_department_row['provstate_id'];
			$company_postal_code=$employer_row['postal_code'];
		}

		if ($province<>NULL){
			$province_sql="Select provstate_name from provstate_list where provstate_id=".$province;
			$province_results=$GLOBALS['dbh']->Execute($province_sql);
			$province_row=$province_results->FetchRow();
			$province=$province_row['provstate_name'];
		}

		if ($country<>NULL){
			$country_sql="Select country_name from country_list where country_id=".$country;
			$country_results=$GLOBALS['dbh']->Execute($country_sql);
			$country_row=$country_results->FetchRow();
			$country=$country_row['country_name'];
		}

		if ($student_row['department_id']<>NULL){
			$department_sql="Select department_name from department where department_id=".$student_row['department_id'];
			$department_results=$GLOBALS['dbh']->Execute($department_sql);
			$department_row=$department_results->FetchRow();
		}

		//start history information output
			$historyOutput .= "<table border='1' cellpadding='10' width='96%'><tr><td>
						<table border='0' cellpadding='5' cellspacing='0' width='100%'>";

			// Student Name
			$historyOutput .= "<tr><td colspan='3' align='center'><h3>&nbsp;". $student_row["first_name"] . " " . $student_row["last_name"]." (".$student_row['student_number'].") at ". $history_row['company_name'] . "</h4><hr /></td></tr>";
			$historyOutput .= "<tr><td colspan='3' align='center'><h3>&nbsp;".$history_row["term_name"] . " " . $history_row["year"]. "</h4><hr /></td></tr>";

			// Didn't want to hardcode column widths.. may change later.
				$col1width = "35%"; $col2width = "35%"; $col3width = "30%";

			/****** Main Info Section of View History
				On Left : Current/Permanent Address, Current/Permanent Phone #, E-mail, High School, Birthdate, Citizenship, Gender.
				On Right : Photo if it exists
			*******/
				// Check Photo's existence.
				if ($student_row["photo"] == NULL) {
					$photo_output = "<b>No Photo Available</b>";
				} else { 
                    // hack to fix an IE image caching bug
                    $num = rand(0,100);

					$photo_uri = $PHP_SELF_MENU."?select=student_photo_viewer&amp;no_html=1&amp;record_id=".$record_id."&amp;num=".$num."' alt='". $student_row["first_name"] . " " . $student_row["last_name"] . "' alt='' />";
                    $photo_local = $student_row['photo'];
					$photo_output = "<img src='".STUDENT_PHOTO_PLACEHOLDER."' alt='". $student_row["first_name"] . " " . $student_row["last_name"] . "' alt='' />";
				}

				// Get Current Address (build the output string..)
					if ($company_street_address_1) $current_address_output .= $company_street_address_1 . "<br />";
					if ($company_street_address_2) $current_address_output .= $company_street_address_2 . "<br />";
					if ($company_city) $current_address_output .= $company_city . ", ";
					if ($company_province)	{	$current_address_output .= $company_province . "<br />"; }
									else {	$current_address_output .=  ($province) ? "<br />" : ""; }
					if ($company_country) $current_address_output .= $company_country;
					if ($company_postal_code)$current_address_output .= "<br />" .  $company_postal_code;

				// Get Permanent Address (build the output string..)
					if ($row["street_address_permanent"]) $permanent_address_output .= $row["street_address_permanent"] . "<br />";
					if ($row["street_address_permanent2"]) $permanent_address_output .= $row["street_address_permanent2"] . "<br />";
					if ($row["city_permanent"]) $permanent_address_output .= $row["city_permanent"] . ", ";
					if ($row["province_permanent"])	{	$permanent_address_output .= $provincepermanentrow["provstate_name"] . "<br />"; }
									else {	$permanent_address_output .=  ($row["city_permanent"]) ? "<br />" : ""; }
					if ($row["country_permanent"]) $permanent_address_output .= $countrypermanentrow["country_name"];
					if ($row["postal_code"])$permanent_address_output .= "<br />" . $row["postal_code_permanent"];

				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Company Name: </b></td>
							<td width='$col2width' align='left'>". $history_row['company_name'] . "&nbsp;</td>
							<td width='$col3width' align='center' valign='middle' rowspan=9>$photo_output</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Department Name: </b></td>
							<td width='$col2width' align='left'>". $history_row['department_name'] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Current Address : </b></td>
							<td width='$col2width' align='left'>". $current_address_output . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Region : </b></td>
							<td width='$col2width' align='left'>". getRegionName($history_row['company_region_id']) . "&nbsp;</td></tr>";

                if (!$history_row['job_code'])
                {
                    $history_row['job_code'] = 'Own Job / Returning Job';
                }

				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Job Code : </b></td>
							<td width='$col2width' align='left'>". $history_row['job_code'] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b> Supervisor: </b></td>
							<td width='$col2width' align='left'>". $history_row["supervisor_name"]."&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor Title : </b></td>
							<td width='$col2width' align='left'>". $history_row['supervisor_position'] . "&nbsp;</td></tr>";

				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor's Phone Number: </b></td>
							<td width='$col2width' align='left'>". $history_row['supervisor_phone_number'] . "&nbsp;</td></tr>";
                $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor's Cell Number: </b></td>
							<td width='$col2width' align='left'>". $history_row['supervisor_cellphone_number'] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor's Fax Number : </b></td>
							<td width='$col2width' align='left'>". $history_row["supervisor_fax_number"] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor's E-mail : </b></td>
							<td width='$col2width' align='left'>". $history_row["supervisor_email"] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Salary : </b></td>
							<td width='$col2width' align='left'>". $history_row["salary"];
                if ($history_row["salary_period_name"])
                {
                    $historyOutput .=  "/".$history_row["salary_period_name"]."&nbsp;";
                }
                $historyOutput .= "</td></tr>";
                $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Hours per Week : </b></td>
                <td width='$col2width' align='left'>". $history_row["work_term_hours_per_week"]."";
                $historyOutput .= "</td></tr>";
                $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Start Date : </b></td>
                <td width='$col2width' align='left'>". $history_row["start_date"]."";
                $historyOutput .= "</td></tr>";
                $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>End Date : </b></td>
                <td width='$col2width' align='left'>". $history_row["end_date"]."";
                $historyOutput .= "</td></tr>";

                $container_mode = useContainers($history_row['student_number'],$history_row['department_id']);
                if ($container_mode == CONTAINER_TERM)
                {
                    $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Workterm Length : </b></td>
                        <td width='$col2width' align='left'>". $history_row["work_term_length"]." month".(($history_row['work_term_length']==1) ? "" : "s");
                    $historyOutput .= "</td></tr>";
                }
                elseif ($container_mode == CONTAINER_HOURS)
                {
                    $total_days = count_days($history_row["start_date"],$history_row["end_date"]);
                    $hours_per_day = $history_row["work_term_hours_per_week"] / 7;
                    $total_hours = round($total_days * $hours_per_day);

                    $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Total Hours : </b></td>
                                        <td width='$col2width' align='left'>". $total_hours."";  
                    $historyOutput .= "</td></tr>";
                } 

			// spacer
				$historyOutput .= "<tr><td colspan=3><br /><hr /><br /></td></tr>";

			/***** Start Student Information
			******/
				// Get Current Address (build the output string..)
					if ($work_term_street_address_1) $address_output .= $work_term_street_address_1 . "<br />";
					if ($work_term_street_address_2) $address_output .= $work_term_street_address_2 . "<br />";
					if ($work_term_city) $address_output .= $work_term_city . ", ";
					if ($work_term_province)	{	$address_output .= $work_term_province . "<br />"; }
									else {	$address_output .=  ($province) ? "<br />" : ""; }
					if ($work_term_country) $address_output .= $work_term_country;
					if ($work_term_postal_code)$address_output .= "<br />" .  $work_term_postal_code;
		 
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Discipline : </b></td>
							<td width='$col2width' align='left'>". $student_row["discipline_name"] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Number : </b></td>
							<td width='$col2width' align='left'>". $history_row["work_term_number"] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Status : </b></td>
							<td width='$col2width' align='left'>". $wt_status_row["history_status_name"] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Student Job Title : </b></td>
							<td width='$col2width' align='left'>". $history_row["job_title"] . "&nbsp;</td></tr>";
		/*
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Option : </b></td>
							<td width='$col2width' align='left'>". $options_row["history_options_name"] . "&nbsp;</td></tr>";
		*/
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Department : </b></td>
							<td width='$col2width' align='left'>". $department_row['department_name'] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Home Address : </b></td>
							<td width='$col2width' align='left'>". $address_output . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Phone Number : </b></td>
							<td width='$col2width' align='left'>". $history_row["student_work_phone"] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Home Phone Number : </b></td>
							<td width='$col2width' align='left'>". $history_row["work_term_home_phone"] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Student E-mail:</b></td>
							<td width='$col2width' align='left'>". $student_row["email"] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Student Work Term E-mail : </b></td>
							<td width='$col2width' align='left'>". $history_row["work_email"] . "&nbsp;</td></tr>";
			// spacer
				$historyOutput .= "<tr><td colspan=3><br /><hr /><br /></td></tr>";

			/***** Start Work Term Information
			******/

				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Student Work Term Visit On : </b></td>
							<td width='$col2width' align='left'>". $history_row["site_visit_date"] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Student Work Term Visit by : </b></td>
							<td width='$col2width' align='left'>". $visitor_row['first_name'] ." ".$visitor_row['last_name']."&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Student Work Term Visit Type: </b></td>
							 <td width='$col2width' align='left'>".$site_visit_type_row['description']."&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor Work Term Visit On : </b></td>
							<td width='$col2width' align='left'>". $history_row["site_visit_date_supervisor"] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor Work Term Visit by : </b></td>
							<td width='$col2width' align='left'>". $supervisor_visitor_row['first_name'] ." ".$supervisor_visitor_row['last_name']."&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor Work Term Visit Type: </b></td>
							 <td width='$col2width' align='left'>".$supervisor_site_visit_type_row['description']."&nbsp;</td></tr>";
                $work_site_visit_notes = preg_replace("/\n/","<br />",$history_row['work_site_visit_notes']);
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Visit Notes : </b></td>
							<td width='$col2width' align='left'>". $work_site_visit_notes. "&nbsp;</td></tr>";
			
			// spacer
				$historyOutput .= "<tr><td colspan=3><br /><hr /><br /></td></tr>";

			/***** Start Work Term Report Information
			******/

				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Report Title : </b></td>
							<td width='$col2width' align='left'>". $history_row["report_title"] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Subject : </b></td>
							<td width='$col2width' align='left'>". $report_subject_row['report_subject_name'] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Report Marker : </b></td>
							<td width='$col2width' align='left'>". $history_row['report_marker'] . "&nbsp;</td></tr>";
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Report Code : </b></td>
							<td width='$col2width' align='left'>". $history_row["report_code"] . "&nbsp;</td></tr>";

			// spacer
				$historyOutput .= "<tr><td colspan=3><br /><hr /><br /></td></tr>";

			// Flags : Student flags
/* Old Code.
				// Table Version of Display
				//  Build Flags Table.
					$flags_output = "<center><b>History Flags : </b><br /><table border=1 cellspacing=0 cellpadding=1 width='75%'><tr><td><table cellspacing=1 cellpadding=3 border=0 width='100%'>";
					$arrFlags=history_flags($history_id,$student_row["department_id"]);
					$count=0;
					foreach ($arrFlags as $x){
						$count=$count+1;
						(($count%2==0) ? $width=30 : $width=3);
						(($count%2==0) ? $class="row0d" : $class="row1d");
						if ($count%6==0){
				
						       $flags_output .= "<td width=$width% class='$class'>$x&nbsp;</td></tr>";
						}else{
							if ($count%6==1){
								$flags_output .= "<tr>";
							}
							$flags_output .= "<td width=$width% class='$class'>$x&nbsp;</td>";
						}
					}
					while ($count%6 != 0) {
						$count++;
						(($count%2==0) ? $width=30 : $width=3);
						(($count%2==0) ? $class="row0d" : $class="row1d");
						$close_tr = 1;
						$flags_output .= "<td width=$width% class='$class'>&nbsp;</td>";
					}
					if ($close_tr = 1) { $flags_output .= "</tr>"; }
					$flags_output .= "</table></td></tr></table>";

				// $historyOutput .= "<tr><td colspan=3>" . $flags_output . "</td></tr>";
					
					$arrFlags=history_flags($history_id,$student_row["department_id"],"> 1");
					$count=0;
					$flags_output = "";
					for ($i = 0; $i < sizeof($arrFlags); $i++) {
						if (substr($arrFlags[$i + 1], 0, 5) != "&nbsp") {
							$flags_output .= $arrFlags[$i] . "<br />";
						}
						$i++;
					}
*/

                    
					// Flags : Student flags
                    $arrFlags=history_flags($history_id,$student_row['department_id']);
                    foreach ($arrFlags as $id => $flag){
                        if ($flag['checked']){
                            $flags_output .= $flag['description']."<br />";
                        }
                    }

                    
                    $historyOutput .= "<tr><td valign='top' align='right'><b>History Flags :</b></td>
								<td>$flags_output</td>";
			
			// spacer
				$historyOutput .= "<tr><td colspan=3><br /><hr /><br /></td></tr>";

			// Last Section : Comments, Notes, Resumes, Coverletters, Work Term History
			
				$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Notes : </b></td>
							</tr>";

                while($rstNotes=$notesresults->FetchRow())
                {
						$historyOutput .= "<tr><td>&nbsp;&nbsp;</td><td>";
						$contact_id_row=getContactInfoByID($rstNotes['contact_id']);
						$contact_name=$contact_id_row['first_name']." ".$contact_id_row['last_name'];
						$historyOutput .= "<b class='blue'>".$contact_name."</b><br /><b class='blue'>".$rstNotes['date_entered']."</b><br /><p><p>";
						$temp=explode("\n",$rstNotes["notes"]);
						$historyOutput .= implode("<br />",$temp);
						$historyOutput .= "</td></tr>";
                        $count=$count+1;
                }

                /*
				$changes_sql = "SELECT changes_recorded_1,changes_recorded_2,changes_recorded_3,change_by_1,change_by_2,change_by_3,change_date_1
						       ,change_date_2,change_date_3 
						FROM history 
						WHERE history_id=".$history_id;
				$changes_results = $GLOBALS['dbh']->Execute($changes_sql);
				$changes_row = $changes_results->FetchRow(); 

				$historyOutput .= "<tr><td colspan=3><br /><hr /><br /></td></tr>";

				if ($changes_row['changes_recorded_1']<>NULL){

					$contact_name= getContactName($changes_row['change_by_1']);	
					$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Fields Changed: </b></td>
						<td width='$col2width' align='left'>". $changes_row['changes_recorded_1'] . "&nbsp;</td></tr>";
					$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Changed By: </b></td>
						<td width='$col2width' align='left'>". $contact_name['first_name']." ".$contact_name['last_name'] . "&nbsp;</td></tr>";
					$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Change Date: </b></td>
						<td width='$col2width' align='left'>". $changes_row['change_date_1'] . "&nbsp;</td></tr>";
						
					if ($changes_row['changes_recorded_2']<>NULL){
						$historyOutput .= "<tr><td colspan=3><br /><hr /><br /></td></tr>";
						$contact_name= getContactName($changes_row['change_by_2']);	
						$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Fields Changed: </b></td>
							<td width='$col2width' align='left'>". $changes_row['changes_recorded_2'] . "&nbsp;</td></tr>";
						$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Changed By: </b></td>
							<td width='$col2width' align='left'>".$contact_name['first_name']." ".$contact_name['last_name'] . "&nbsp;</td></tr>";
						$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Change Date: </b></td>
							<td width='$col2width' align='left'>". $changes_row['change_date_2'] . "&nbsp;</td></tr>";
					}
					if ($changes_row['changes_recorded_3']<>NULL){
						$historyOutput .= "<tr><td colspan=3><br /><hr /><br /></td></tr>";
						$contact_name= getContactName($changes_row['change_by_3']);	
						$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Fields changed: </b></td>
							<td width='$col2width' align='left'>". $changes_row['changes_recorded_3'] . "&nbsp;</td></tr>";
						$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Changed By: </b></td>
							<td width='$col2width' align='left'>". $contact_name['first_name']." ".$contact_name['last_name'] . "&nbsp;</td></tr>";
						$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Change Date: </b></td>
							<td width='$col2width' align='left'>". $changes_row['change_date_3'] . "&nbsp;</td></tr>";
					}
				}
                */

			// End of Student Info Table
            
			$historyOutput .= "</table></td></tr></table><!-- PAGE BREAK -->";
            $pdfOutput .= $historyOutput;
    } //  End foreach
			#echo $historyOutput;
			
            echo("<table>");
            $historyOutput = str_replace(STUDENT_PHOTO_PLACEHOLDER, $photo_uri, $historyOutput);
            $pdfOutput = str_replace(STUDENT_PHOTO_PLACEHOLDER, $photo_local, $pdfOutput);
 
            echo("<td align'left'><form method='post' action='" . $PHP_SELF . "'>
                    <input type='hidden' name='select' value='historyOutput' />
                    <input type='hidden' name='HTMLhistory' value='". urlencode($pdfOutput) . "' />
                    <input type='hidden' name='PDF' value='1' />
                    <input type='submit' name='viewpdf' value='View in PDF' />
                    </form>
                    </td>");
            echo("</table>");
            ?>

