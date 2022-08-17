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
 | Filename: check_input.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: Check the inputs of the edited record                           |
 +------------------------------------------------------------------------------+

*/

$stack = array();
if ($editHistory->term_id<>$row['term_id'] || $editHistory->year<>$row['year']){
	$changes['term_id']="<b class='black'>(".getTermName($editHistory->term_id)." ".$editHistory->year.")</b>";
	$errorlevel=1;
	array_push($stack,1);
}
if ($editHistory->company_name<>$row['company_name']){
	$changes['company_name']="<b class='black'>(".$editHistory->company_name.")</b>";
	$errorlevel=1;
	array_push($stack,2);
}
if ($editHistory->department_name<>$row['department_name']){
	$changes['department_name']="<b class='black'>(".$editHistory->department_name.")</b>";
	$errorlevel=1;
	array_push($stack,3);
}
if ($editHistory->company_street_address_1<>$row['company_street_address_1']){
	$changes['company_street_address_1']="<b class='black'>(".$editHistory->company_street_address_1.")</b>";
	$errorlevel=1;
	array_push($stack,4);
}
if ($editHistory->company_street_address_2<>$row['company_street_address_2']){
	$changes['company_street_address_2']="<b class='black'>(".$editHistory->company_street_address_2.")</b>";
	$errorlevel=1;
	array_push($stack,5);
}
if ($editHistory->company_street_address_3<>$row['company_street_address_3']){
	$changes['company_street_address_3']="<b class='black'>(".$editHistory->company_street_address_3.")</b>";
	$errorlevel=1;
	array_push($stack,6);
}
if ($editHistory->company_city<>$row['company_city']){
	$changes['company_city']="<b class='black'>(".$editHistory->company_city.")</b>";
	$errorlevel=1;
	array_push($stack,7);
}
if ($editHistory->company_province<>$row['company_province']){
	$changes['company_province']="<b class='black'>(".getProvstateName($editHistory->company_province).")</b>";
	$errorlevel=1;
	array_push($stack,8);
}
if ($editHistory->company_country<>$row['company_country']){
	$changes['company_country']="<b class='black'>(".getCountryName($editHistory->company_country).")</b>";
	$errorlevel=1;
	array_push($stack,9);
}
if ($editHistory->company_region<>$row['company_region_id'])
{
    $changes['company_region_id']="<b class='black'>(".getRegionName($editHistory->company_region).")</b>";
    $errorlevel=1;
    array_push($stack,10);
}

if ($editHistory->company_postal_code<>$row['company_postal_code']){
	$changes['company_postal_code']="<b class='black'>(".$editHistory->company_postal_code.")</b>";
	$errorlevel=1;
	array_push($stack,11);
}
if ($editHistory->supervisor_name<>$row['supervisor_name']){
	$changes['supervisor_name']="<b class='black'>(".$editHistory->supervisor_name.")</b>";
	$errorlevel=1;
	array_push($stack,12);
}
if ($editHistory->supervisor_fax_number<>$row['supervisor_fax_number']){
	$changes['supervisor_fax_number']="<b class='black'>(".$editHistory->supervisor_fax_number.")</b>";
	$errorlevel=1;
	array_push($stack,13);
}
if ($editHistory->supervisor_phone_number<>$row['supervisor_phone_number']){
	$changes['supervisor_phone_number']="<b class='black'>(".$editHistory->supervisor_phone_number.")</b>";
	$errorlevel=1;
	array_push($stack,14);
}
if ($editHistory->supervisor_cellphone_number<>$row['supervisor_cellphone_number']){
	$changes['supervisor_cellphone_number']="<b class='black'>(".$editHistory->supervisor_cellphone_number.")</b>";
	$errorlevel=1;
	array_push($stack,14);
}
if ($editHistory->supervisor_email<>$row['supervisor_email']){
	$changes['supervisor_email']="<b class='black'>(".$editHistory->supervisor_email.")</b>";
	$errorlevel=1;
	array_push($stack,15);
}
if ($editHistory->supervisor_position<>$row['supervisor_position']){
	$changes['supervisor_position']="<b class='black'>(".$editHistory->supervisor_position.")</b>";
	$errorlevel=1;
	array_push($stack,16);
}

if ($editHistory->salary<>$row['salary']){
	$changes['salary']="<b class='black'>(".$editHistory->salary.")</b>";
	$errorlevel=1;
	array_push($stack,17);
}
if ($editHistory->salary_period_id<>$row['salary_period_id']){
	$changes['salary_period_id']="<b class='black'>(".getSalaryPeriodName($editHistory->salary_period_id).")</b>";
	$errorlevel=1;
	array_push($stack,"17a");
}
if ($editHistory->work_term_hours_per_week<>$row['work_term_hours_per_week']){
	$changes['work_term_hours_per_week']="<b class='black'>(".$editHistory->work_term_hours_per_week.")</b>";
	$errorlevel=1;
	array_push($stack,"17b");
}
if ($editHistory->start_date<>$row['start_date']){
	$changes['start_date']="<b class='black'>(".$editHistory->start_date.")</b>";
	$errorlevel=1;
	array_push($stack,"17c");
}
if ($editHistory->end_date<>$row['end_date']){
	$changes['end_date']="<b class='black'>(".$editHistory->end_date.")</b>";
	$errorlevel=1;
	array_push($stack,"17d");
}
if ($editHistory->work_term_length<> $row['work_term_length']) {
    $changes['work_term_length'] = "<b class='black'>(".$editHistory->work_term_length." month".($editHistory->work_term_length == 1 ? "" : "s").")</b>";
    $errorlevel=1;
    array_push($stack,"17e");
}
if ($editHistory->wt_status<>$row['wt_status']){
	$changes['wt_status']="<b class='black'>(".getHistoryStatus($editHistory->wt_status).")</b>";
	$errorlevel=1;
	array_push($stack,18);
}
if ($editHistory->job_title<>$row['job_title']){
	$changes['job_title']="<b class='black'>(".$editHistory->job_title.")</b>";
	$errorlevel=1;
	array_push($stack,19);
}
if ($editHistory->work_term_street_address_1<>$row['work_term_street_address_1']){
	$changes['work_term_street_address_1']="<b class='black'>(".$editHistory->work_term_street_address_1.")</b>";
	$errorlevel=1;
	array_push($stack,20);
}
if ($editHistory->work_term_street_address_2<>$row['work_term_street_address_2']){
	$changes['work_term_street_address_2']="<b class='black'>(".$editHistory->work_term_street_address_2.")</b>";
	$errorlevel=1;
	array_push($stack,21);
}
if ($editHistory->work_term_street_address_3<>$row['work_term_street_address_3']){
	$changes['work_term_street_address_3']="<b class='black'>(".$editHistory->work_term_street_address_3.")</b>";
	$errorlevel=1;
	array_push($stack,22);
}
if ($editHistory->work_term_city<>$row['work_term_city']){
	$changes['work_term_city']="<b class='black'>(".$editHistory->work_term_city.")</b>";
	$errorlevel=1;
	array_push($stack,23);
}
if ($editHistory->work_term_postal_code<>$row['work_term_postal_code']){
	$changes['work_term_postal_code']="<b class='black'>(".$editHistory->work_term_postal_code.")</b>";
	$errorlevel=1;
	array_push($stack,24);
}
if ($editHistory->work_term_province<>$row['work_term_province']){
	$changes['work_term_province']="<b class='black'>(".getProvstateName($editHistory->work_term_province).")</b>";
	$errorlevel=1;
	array_push($stack,25);
}
if ($editHistory->work_term_country<>$row['work_term_country']){
	$changes['work_term_country']="<b class='black'>(".getCountryName($editHistory->work_term_country).")</b>";
	$errorlevel=1;
	array_push($stack,26);
}
if ($editHistory->work_term_home_phone<>$row['work_term_home_phone']){
	$changes['work_term_home_phone']="<b class='black'>(".$editHistory->work_term_home_phone.")</b>";
	$errorlevel=1;
	array_push($stack,27);
}
if ($editHistory->student_work_phone<>$row['student_work_phone']){
	$changes['student_work_phone']="<b class='black'>(".$editHistory->student_work_phone.")</b>";
	$errorlevel=1;
	array_push($stack,28);
}
if ($editHistory->work_email<>$row['work_email']){
	$changes['work_email']="<b class='black'>(".$editHistory->work_email.")</b>";
	$errorlevel=1;
	array_push($stack,29);
}
if ($editHistory->site_visit_date_supervisor<>$row['site_visit_date_supervisor']){
	$changes['site_visit_date_supervisor']="<b class='black'>(".$editHistory->site_visit_date_supervisor.")</b>";
	$errorlevel=1;
	array_push($stack,30);
}
if ($editHistory->site_visit_date<>$row['site_visit_date']){
	$changes['site_visit_date']="<b class='black'>(".$editHistory->site_visit_date.")</b>";
	$errorlevel=1;
	array_push($stack,31);
}
if ($editHistory->site_visit_type_id<>$row['site_visit_type_id']){
	$changes['site_visit_type_id']="<b class='black'>(".getSiteVisitType($editHistory->site_visit_type_id).")</b>";
	$errorlevel=1;
	array_push($stack,32);
}
if ($editHistory->site_visit_type_id_supervisor<>$row['site_visit_type_id_supervisor']){
	$changes['site_visit_type_id_supervisor']="<b class='black'>(".getSiteVisitType($editHistory->site_visit_type_id_supervisor).")</b>";
	$errorlevel=1;
	array_push($stack,33);
}
if ($editHistory->site_visit_by<>$row['site_visit_by']){
	$arrSiteVisitBy=getContactName($editHistory->site_visit_by);
	$changes['site_visit_by']="<b class='black'>(".$arrSiteVisitBy['first_name']." ".$arrSiteVisitBy['last_name'].")</b>";
	$errorlevel=1;
	array_push($stack,34);
}
if ($editHistory->site_visit_by_supervisor<>$row['site_visit_by_supervisor']){

	$arrSiteVisitBy=getContactName($editHistory->site_visit_by_supervisor);
	$changes['site_visit_by_supervisor']="<b class='black'>(".$arrSiteVisitBy['first_name']." ".$arrSiteVisitBy['last_name'].")</b>";
	$errorlevel=1;
	array_push($stack,35);
}
if ($editHistory->work_site_visit_notes<>$row['work_site_visit_notes']){
	$changes['work_site_visit_notes']="<b class='black'>(".$editHistory->work_site_visit_notes.")</b>";
	$errorlevel=1;
	array_push($stack,36);
}
if ($editHistory->report_title<>$row['report_title']){
	$changes['report_title']="<b class='black'>(".$editHistory->report_title.")</b>";
	$errorlevel=1;
	array_push($stack,37);
}
if ($editHistory->report_code<>$row['report_code']){
	$changes['report_code']="<b class='black'>(".$editHistory->report_code.")</b>";
	$errorlevel=1;
	array_push($stack,38);
}
if ($editHistory->report_subject<>$row['report_subject']){
	$changes['report_subject']="<b class='black'>(".getHistoryReportSubject($editHistory->report_subject).")</b>";
	$errorlevel=1;
	array_push($stack,39);
}
if ($editHistory->report_marker<>$row['report_marker']){
	$changes['report_marker']="<b class='black'>(".$editHistory->report_marker.")</b>";
	$errorlevel=1;
	array_push($stack,40);
}
    foreach ($flags as $id => $flag){
        if (($editHistory->flags[$id]->checked && $flag->checked) || (!$editHistory->flags[$id]->checked && !$flag->checked)){
            // Do nothing
        }
        if ((!$editHistory->flags[$id]->checked && $flag->checked) || ($editHistory->flags[$id]->checked && !$flag->checked)){ 
            $changes['flags'] .= "<b class='black'>(".$flag['description'].")</b><br />"; 
            $errorlevel = 1; 
        } 
    }
//old code
if(0){
    $errorlevel=1;
	if ($editHistory->flags<>NULL){
		array_push($stack,"42a");
		$count = 0;
		foreach ($editHistory->flags as $x){
			$changes['flags'] =  $changes['flags']."<b class='black'>(".$x.")</b><br />";
		}
	}else{
		$changes['flags']="<b class='black'>()</b>";
		array_push($stack,"42b");
	}
}

	//for logging errors
        if($errorlevel == 1) {

		$fp = fopen(PATH_LOGS . "edit_student.log", "a");

		//logs $record_id and $student_department_row["department_id"] to search for bugs

		$str = ("\n*********************************************************\n");
		$sql="SELECT CURRENT_DATE as date, CURRENT_TIME as time";
		$result=$GLOBALS['dbh']->Execute($sql);
		while ($row=$result->FetchRow()){$date=$row['date'];$time=$row['time'];}
		
		$str .= ("$date : $time : $auth->name : $REMOTE_ADDR\n");
		if( trim( $QUERY_STRING ) ){
			$query = stripslashes($QUERY_STRING);
			$str .= "\tURL = $query\n";
		}
		$str .= "\tBROWSER/PLATFORM = $HTTP_USER_AGENT\n";

		$str .= ("\trecord_id: $record_id\n");
		$str .= ("\tstudent_department_row:" .  $student_department_row["department_id"] . "\n");

		$str .= ("\tError flags: ");

		foreach ($stack as $value) {
			$str .= $value . " ";
		}

		$str .= "\n";
		$str .= ("*********************************************************\n");
		$str .= ("\n");

		fputs($fp, $str);
	}
?>
