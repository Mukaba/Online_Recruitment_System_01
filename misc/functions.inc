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
 | Filename: functions.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description: This file contains all (or most of) the generic functions       |
 | used by the system. functions in this file...                                |
 |                                                                              |
 | check_name_admin, check_name_i, check_name_is, formatLongDate,               |
 | formatStandardDate, success_check, trim_elements, unique_arrays,             |
 | unique_elements,                                                             |
 |                                                                              |
 | str getCompanyAndDivisionName ($contact_id) // if the company name is the    |
 | same as the division name, return only one, otherwise both str               |
 | formatTime(str $time, [boolean $leading_space]) str txtFormatTime(str        |
 | $time) boolean isNumber(var $value) boolean isInteger(var $value) boolean    |
 | isValidJobID(int $job_id) boolean isValidJobCode(str $job_code) boolean      |
 | isValidTimeID(int $time_id, [int $job_id, [boolean $interviews_only,         |
 | [boolean $no_switch]]]) boolean isStudentShortlisted(int $job_id, str        |
 | $student_number) boolean isSignUpComplete(int $job_id) str unslash(str       |
 | $string) boolean isValidDate(str $sdate) boolean isValidEmployerID(int       |
 | $employer_id) boolean isValidCompanyName(str $company_name) boolean          |
 | isValidStudentNumber(str $student_num) boolean isValidStudentName(str        |
 | $student_name) boolean isValidPhoneNumber(str $phone_number) void            |
 | notify(str $text) void error(str $text) void instruct(str $text) void        |
 | info(str $text) boolean isStudentSignedUp(int $job_id, str $student_number)  |
 | boolean isJobCancelled(int $job_id) boolean isShowable(int $job_id) boolean  |
 | isJobIDStudentEditable(int $job_id, str $student_number)		// requires     |
 | isEmailSent.inc boolean isUnsignedTimeID(int $time_id) boolean               |
 | isJobMultipleDays(int $job_id) Array getDateTime(int $time_id) ternary       |
 | isSwitchingInterview(int $job_id)	// requires constants.inc Array         |
 | isTimeConflict(str $student_number, int $time_id)		// requires         |
 | constants.inc ternary isOffCampusPhone(int $job_id)		// requires         |
 | constants.inc Array getInterviewPhone(int $job_id, str $student_number)	//  |
 | requires constants.inc Array getTimeID(int $job_id, str                      |
 | $student_number)				// requires constants.inc Array                 |
 | getSequentialEndTime(int $time_id) Array getUnsignedStudents(int $job_id)    |
 | boolean getStudentName(str $student_number) boolean getStudentEmail(str      |
 | $student_number) str getCiEmail(str $login_id)   ... get contact internal    |
 | e-mail str getCiEmailUsingID(str $contact_id)  Get the internal contact's    |
 | e-mail using their contact_id. str getContactID(str $department, str         |
 | $login)  Get the contact_id associated with the given dept and login_id.     |
 | str getContactInfoByID(str $contact_id) Get the internal contact             |
 | information using their contact_id. str clock()	... returns HTML clock code |
 | Array break_names(Array $names) boolean isOnCampus(int $interview_place_id)  |
 | boolean isPhone(int $interview_medium_id) boolean isVideo(int                |
 | $interview_medium_id) boolean isSwitching(int $interview_type_id) boolean    |
 | isSimultaneous(int $interview_type_id) boolean isSequential(int              |
 | $interview_type_id) boolean isJobInfoEditable(int $job_id) time              |
 | interviewDate(str $job_code) boolean isNewsGroupEmailSent(int $job_id)       |
 | Array trim_array(Array $trim) string getAreaName(int $area_id) string        |
 | getAreaNameFromCountry(int $country_id) string                               |
 | getCountryName(int $country_id) string getProvstateName(int $provstate_id)   |
 | string getRegionName(int $region_id) string getTermName(int $term_id)        |
 | string getSizeRange(int $size_id) string getFlagName(int $flag_id, string    |
 | $flag_type) string flagInDept(int $flag_id, int $department_id,              |
 | $contact_dept_comp) string normalizeWebsite(string $website) string          |
 | encodesql (string $s) string decodesql (string $s) array                     |
 | recordContactEdit($editedContact) boolean recordChange($newvalue,            |
 | $oldvalue) string validDate(string $date) string validTime(string $time)     |
 | boolean checkDate(string $date) string add_dates(string $date1, string       |
 | $date2) sorted_array sortName(array $names) string cal_length(string         |
 | $time1, string $time2) string add_times(string $time1, string $time2)        |
 | string sub_times(string $time1, string $time2) boolean isDuplicate(array     |
 | $param, string &$duplicate) void check_student_name(string &$students)       |
 | boolean isSlotsEnough(array $schedules, array $students, $sim_or_swi =       |
 | null) void remove_element(array $array, int $element) void                   |
 | print_array(array $array) void update_duplicate(string $select) void         |
 | name_value_pair(strin $name, array $array, array $array2) void               |
 | displayFormElement() int findStudentNumbers(string $name) array              |
 | getNextTermYear() array getNextTermYear2($term_id, $year, $order = 'year')   |
 | array getPreviousTermYear() int getCurrentTermID() int getCurrentYear()      |
 | boolean isValidNetlink(int $netlink_id) boolean isValidNIS(int $login_id)    |
 | int getStudentEmailDepartment(string $student_number) boolean                |
 | isApplicableDept(int $job_id, int $department_id) boolean                    |
 | canStudentApply(int $job_id, int $student_number) string                     |
 | reasonStudentCantApply(int $job_id, int $student_number) string              |
 | generatePassword(int $size) string getStatusName(int $status_id) string      |
 | getEmployerStatusName(int $status_id) string getAdminStatusName(int          |
 | $status_id) string getHistoryStatusName(int $status_id) int_array            |
 | department_groups($department_id) string getCompanyName($employer_id)        |
 | string getDivisionName($division_id) string_array                            |
 | getContactName($contact_id) (array holds first and last name) string         |
 | getTitleName($title_id) string further_progress_possible($job_id,            |
 | $student_number) boolean employer_not_hiring($job_id) boolean                |
 | is_job_flagged($job_id, $student_number) string getCurrentTime() string      |
 | getJobCode($job_id) int getJobEmployer($job_id) string_array                 |
 | getIntJobCode($job_id) boolean hasStudentApplied(int $job_id, int            |
 | $student_number) boolean hasStudentBeenPlacedInJob(int $job_id, int          |
 | $student_number) string_array getDeptArray($job_id) string_array             |
 | getDeptCodeArray($job_id) string_array getDiscArray($job_id) string          |
 | getDisciplineCode($discipline_id) string getDiscString($job_id) string       |
 | pulldownGroup($departments_in_group) string getAdminStatusGif($job_id) int   |
 | getFilledDept($job_id) array of ints adminStatusString($job_id) string       |
 | getHistoryStatus($id); string getHistoryOptions($id); string                 |
 | getHistoryReportSubject($id); string getSiteVisitType($id); boolean          |
 | JobhasInterview (int $job_id) int getShortlistContactfromJobID (int          |
 | $job_id) string getHoldTimeRemaining (string $hold_begin_datetime) string    |
 | getHoldEndTime () string getDepartmentName (int $department_id) string       |
 | getDepartmentCode (int $department_id) string getDisciplineName (int         |
 | $discipline_id) string strip_html_tags ($str) int[] getStudentDepartments    |
 | (int $student_number, boolean include_groups = false) int[]                  |
 | getDisciplinesForDepts (string/int[] $dept_ids) boolean contactHasJobs (int  |
 | $contact_id) boolean array_key_exists (mixed $key, array $array) array       |
 | removeSlashes (string or array of strings) array getJobsWithContact(int      |
 | $contact_id) array getJobsWithDivision(int $division_id) string              |
 | getSemesterName(int $semester_id) int                                        |
 | getSpecialTermYear ()                                                        |
 +------------------------------------------------------------------------------+

*/

//:TODO make sure this is actually needed.
// need this for some of the classes to know if a var is a class or not
// the is_a function comes in php ver. 4.2.0
if (!function_exists('is_a')) {
    function is_a($classname, $classquery) {

        $father = get_parent_class($classname);

        if (strtolower($father) == strtolower($classquery)) {
            return true;
        }
        elseif (!empty($father)) {
            return is_a($father, $classquery);
        }
        else {
            return false;
        }
    }
}

// {{{ Interview Functions
// {{{ function JobHasInterview($job_id)

/**
 * See if a job has an interview
 */
function JobHasInterview($job_id)
{

	// first let's check if there's a direct corolation
	$sql = "SELECT DISTINCT i.job_id
		FROM interview AS i
		WHERE i.job_id = '$job_id'";
	$result = $GLOBALS['dbh']->Execute ($sql);
	if ($result->RecordCount())
		return true;
	
	// if not, it doesn't necessarily mean there isn't one, next check
	// if it's a part of a multiple job interivew, more complicated
	else {
		$sql = "SELECT DISTINCT i.job_id
			FROM interview_job_join AS ijj, interview AS i
			WHERE ijj.job_id = '$job_id'
			AND ijj.interview_job_id = i.job_id";
		$result = $GLOBALS['dbh']->Execute ($sql);
		if ($result->RecordCount())
			return true;
	
		// otherwise there ain't one
		else
			return false;
	}
}
// }}}
// {{{ function getShortlistContactfromJobID($job_id)

/**
 * @return mixed The primary contact_id for a job using it's job_id, else false.
 */
function getShortlistContactfromJobID($job_id)
{
	$sql = "SELECT DISTINCT d.shortlist_contact_id
		FROM department AS d, job_info AS ji
		WHERE d.department_id = ji.department_id
		AND ji.job_id = '$job_id'";
	$result = $GLOBALS['dbh']->Execute ($sql);
	if ($row = $result->FetchRow())
		return ($row["shortlist_contact_id"]);
	else
		return FALSE;
}
// }}}
// {{{ function isOnCampus($interview_place_id)
/**
 * Returns true if the interview_place_id represents an oncampus interview 
 * ONCAMPUS is defined in constants.inc
 */
function isOnCampus($interview_place_id){
	$sql = "SELECT description
		FROM interview_place
		WHERE interview_place_id = $interview_place_id";
	$result = $GLOBALS['dbh']->Execute($sql);
	if (!$result){ return false; }

	$row = $result->FetchRow();
	if ($row["description"] == ONCAMPUS){
		$result->Close();
		return true;
	}else{
		$result->Close();
		return false;
	}
}

// }}}
// {{{ function interviewDatetime($job_code)
/**
 * @return string The last interview time with job_code $job_code
 */
function interviewDatetime($job_code)
{
	$sql = ("
		SELECT MAX(t.int_date) AS date
                FROM interview_time AS t, job_info as j
                WHERE t.job_id = j.job_id 
		AND j.job_code = '" . addslashes($job_code) . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	if (!$result)
	{
		return;
	}
	
	$row = $result->FetchRow();
	$max_date = $row["date"];
	
	/*
	 Get the latest time for the interview with this job_code and at this date.
	*/

	$sql = ("
		SELECT MAX(t.end_time) AS time
		FROM interview_time AS t, job_info AS j
		WHERE t.job_id=j.job_id
		AND t.int_date='" . addslashes($max_date) . "'
		AND j.job_code='" . addslashes($job_code) . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if (!$result)
	{
		return;
	}

	$row = $result->FetchRow();
	$max_time = $row["time"];

	$max_date_time = ($max_date . " " . $max_time);
	
	return $max_date_time;
}

// }}}
// {{{ function interviewDate($job_code)
/**
 * @return string The last interview date with job_code $job_code
 */
function interviewDate($job_code)
{
	$sql = "SELECT MAX(t.int_date) AS time
		FROM interview_time AS t, job_info as j
		WHERE t.job_id = j.job_id AND
		j.job_code = '" . addslashes($job_code) . "'";
		
	$result = $GLOBALS['dbh']->Execute( $sql );
	
	if (!$result)
	{
		return;
	}
	else
	{
		$row = $result->FetchRow();
		$result->Close();	
		return $row["time"];
	}
}

// }}}
// {{{ function getInterviewPhone(int $job_id, str $student_number)
/**
 * @return array Array(ternary "return", str "value")
 *	             return=T_TRUE, value=phone number - phone number for $time_id
 *			 	 return=T_FALSE, value="" - no phone number for $time_id
 *			 	 return=T_ERROR, value=error description - query failed
 */
function getInterviewPhone($job_id, $student_number)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	b.phone
		FROM	interview_time AS a, interview_phone AS b
		WHERE	a.time_id=b.time_id AND
			a.job_id='" . addslashes($job_id) . "' AND
			a.student_number='" . addslashes($student_number) . "'
			AND NOT a.switch
			AND NOT a.sequential
			");

	if (!$result) { return Array("return"=>T_ERROR, "value"=>"Query failed. "); }
	
	$rows = $result->RecordCount();
	if ($rows == 0)
	{
		$isInterviewPhone = Array("return"=>T_FALSE, "value"=>"");
	}
	elseif ($rows == 1)
	{
		$row = $result->FetchRow();
		$isInterviewPhone = Array("return"=>T_TRUE, "value"=>$row["phone"]);
	}
	else
	{
		$isInterviewPhone = Array("return"=>T_ERROR, "value"=>"Query returned multiple rows. "); 		
	}
	$result->Close();
	
	return $isInterviewPhone;
}

// }}}
// {{{ function getSwitchInterview(int $time_id)
/**
 * @return boolean $time_id of switching complement
 *   			   false - on failed query or $time_id is not switching
 */
function getSwitchInterview($time_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	a.switch, a.job_id, a.int_date, a.int_time, a.interviewer_id
		FROM		interview_time AS a, interview AS b, interview_type as c
		WHERE		a.time_id='" . addslashes($time_id) . "' AND
					a.job_id=b.job_id AND
					c.interview_type_id = b.interview_type_id AND
					c.description = '" . SWITCHING . "' AND
					a.int_type=1");
		
	if (!$result) { return false; }
	if (($result->RecordCount() == 1) && ($row = $result->FetchRow()))
	{
		if ($row["switch"])
		{
			// looking for an earlier time
			$time = "before";
		}
		else
		{
			// looking for a later time
			$time = "after";
		}
		$job_id = $row["job_id"];
		$int_date = $row["int_date"];
		$int_time = $row["int_time"];
		$interviewer_id = $row["interviewer_id"];
	}
	$result->Close();
	if (($time != "after") && ($time != "before")) { return false; }

	$result = $GLOBALS['dbh']->Execute("
		SELECT	time_id
		FROM	interview_time
		WHERE	job_id='" . addslashes($job_id) . "' AND
			int_date='" . addslashes($int_date) . "' AND
			(TIME_FORMAT(int_time, '%k%i%S') + 0)" . (($time == "after") ? ">" : "<") . "(TIME_FORMAT('" . addslashes($int_time) . "', '%k%i%S') + 0) AND
			interviewer_id!='" . addslashes($interviewer_id) . "' AND
			int_type=1
		ORDER BY int_time " . (($time == "after") ? "ASC" : "DESC") . "
		LIMIT 0,1
			");
	if (!$result) { return false; }
	if (($result->RecordCount() == 1) && ($row = $result->FetchRow()))
	{
		$getSwitchInterview = $row["time_id"];
	}
	else
	{
		$getSwitchInterview = false;
	}
	$result->Close();
	
	return $getSwitchInterview;
}

// }}}
// {{{ function isTimeConflict(str $student_number, int $time_id)
/**
 * Checks to see if the student has conflicting interview times.
 * @param int $student_number The student to check for.
 * @param int $time_id A time ID to check for conflicts against. 
 * @return array Array("return", "value")
 *				"return"=>T_TRUE, "value"=>job_code of conflict		- time_conflict
 *				"return"=>T_FALSE, "value"=>""				- not time_conflict
 *				"return"=>T_ERROR, "value"=>description of error	- query failed
 */
function isTimeConflict($student_number, $time_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT a.int_date, a.int_time AS start_time, a.end_time, b.interview_type_id
		FROM interview_time AS a, interview AS b
		WHERE a.time_id='" . addslashes($time_id) . "'
		AND b.job_id=a.job_id
		");

	if (!$result) { return Array("return"=>T_ERROR,"value"=>"Query failed."); }
	if (($result->RecordCount() == 1) && ($row = $result->FetchRow()))
	{
		$int_date = $row["int_date"];
		$start_time = $row["start_time"];
		$end_time = $row["end_time"];

		/*
		In the case of switching interviews the end time of this time_id is not the end time of the interview
		as the student will be signed up for a interview time slot with the second interviewer which is another
		time_id.  I have set the end time to be the start_time + 2 * (end_time - start_time) which should cover
		the entire length that the student is being interviewed for.
		*/

		if (isSwitching($row["interview_type_id"])){
			$int_length = cal_length($start_time, $end_time);
			$total_int_length = 2 * $int_length;
			$end_time = add_times($start_time, $total_int_length);
		}
	
		/*
		 If this is a sequential interview we are dealing with, the end time is not the end time given by the
		 current time_id.  We need to draw the end time of the last sequential interview_time on this date.
		 Procedure: Get the stagger amount (end_time - start_time of first interview of the day).  Find out
		 How many interviewers there are in total for this interview.  Multiply the stagger amount by the
		 number of interviewers.  Add this new amount to the old end time.  We now have the proper end time
		 for a sequential interview on this day.
		*/
		
		if (isSequential($row["interview_type_id"]))
		{
			
			$stagger_amount = cal_length($start_time, $end_time); 
				
			$sql = ("
				SELECT DISTINCT job_id 
				FROM interview_time
				WHERE time_id='" . addslashes($time_id) . "'
				");
			$seq_result = $GLOBALS['dbh']->Execute($sql);
			$seq_row = $seq_result->FetchRow();
			$seq_result->Close();

			$sql = ("
				SELECT DISTINCT sequential
				FROM interview_time
				WHERE job_id='" . $seq_row["job_id"] . "'
				ORDER BY sequential DESC
				");
			$seq_result = $GLOBALS['dbh']->Execute($sql);

			/*
			 We only want the first row from this result, as that will contain the largest value for
			 sequential.
			*/

			$seq_row = $seq_result->FetchRow();
			$seq_result->Close();
			$num_times = $seq_row["sequential"];
			
			for ($i = 0; $i < $num_times; $i++)
			{
				$end_time = add_times($end_time, $stagger_amount);
			}

			/*
			 We have now set the correct end_time.
			*/
		}

	}		
	else
	{
		$result->Close();
		return Array("return"=>T_ERROR,"value"=>"Timeslot not found.");
	}
	$result->Close();
	
	$result = $GLOBALS['dbh']->Execute("
		SELECT	DISTINCT a.job_id, c.job_code
		FROM		interview_time AS a,
				interview AS b,
				job_info as c
		WHERE		a.job_id=b.job_id AND
				c.job_id = a.job_id AND
				a.student_number='" . addslashes($student_number) . "' AND
					NOT b.cancelled AND
					a.int_date='" . addslashes($int_date) . "' AND
					(
						((a.int_time>='" . addslashes($start_time) . "') AND (a.int_time<'" . addslashes($end_time) . "'))
						OR
						((a.end_time>'" . addslashes($start_time) . "') AND (a.end_time<='" . addslashes($end_time) . "'))
						OR
						((a.int_time<'" . addslashes($start_time) . "') AND (a.end_time>'" . addslashes($end_time) . "'))
					)");

	if (!$result) { return Array("return"=>T_ERROR,"value"=>"Query failed."); }
	if (!$result->RecordCount())
	{
		$isTimeConflict = Array("return"=>T_FALSE,"value"=>"");
	}
	else
	{
		$conflicts = "";
		while($row = $result->FetchRow())
		{
			$conflicts .= $row["job_code"] . ", ";
		}
		$conflicts = substr($conflicts, 0, strlen($conflicts) - 2);
		
		$isTimeConflict = Array("return"=>T_TRUE,"value"=>$conflicts);
	}
	$result->Close();
	
	return $isTimeConflict;
}

// }}}
// {{{ function isJobMultipleDays(int $job_id)
/**
 * @return boolean true - if there are multiple interviews on different days for the given job_id.
 *		           false - if the given job_id has either 0 or 1 days worth
 *		           of interviews, or isn't a valid job_id
 * @author Added by Adam on 2001-01-25
 */
function isJobMultipleDays($job_id)
{
	if (trim($job_id) == "")
	{
		return false;
	}
	$result = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT int_date
		FROM interview_time
		WHERE job_id ='" . addslashes($job_id) . "'
		");
	if (!($result)) 
	{
		return false;
	}
	$isJobMultipleDays = ($result->RecordCount() > 1);
	$result->Close();
	return $isJobMultipleDays;
}

// }}}
// {{{ function isStudentShortlisted(int $job_id, str $student_number)
/**
 * This has been modified.  We are now also checking to see if this is just a job part of a multi
 * job interview.  If that is the case, we will need to pull out the interview job id instead.
 * @return boolean:	true  - if $student_number is shortlisted for $job_id
 *				    false - if $student_number is not shortlisted for $job_id
 */
function isStudentShortlisted($job_id, $student_number)
{
	$sql = ("
		SELECT DISTINCT interview_job_id
		FROM interview_job_join
		WHERE job_id='" . $job_id . "'
        ");
	$result = $GLOBALS['dbh']->Execute($sql);
	if ($result->RecordCount())
	{
		// This is a job that is part of a multi-job interview.
		$row = $result->FetchRow();
		$job_id = $row["interview_job_id"];
	}

	$result = $GLOBALS['dbh']->Execute("
		SELECT student_number
		FROM students_shortlisted
		WHERE job_id='" . addslashes($job_id) . "' AND student_number='" . addslashes($student_number) . "'");

	if (!$result) { return false; }
	$isStudentShortlisted = (($row = $result->FetchRow()) && ($row["student_number"] == $student_number));
	$result->Close();
	return $isStudentShortlisted;
}

// }}}
// {{{ function isSwitchingInterview(int $job_id)
/**
 * @return int T_TRUE  - if $job_id has switching interviews
 *             T_FALSE - if $job_id doesn't have switching interviews
 *             T_ERROR - if a query failed or $job_id doesn't exist
 */
function isSwitchingInterview($job_id)
{
	$sql = "SELECT job_id
		FROM interview, interview_type
		WHERE interview.job_id = '" . addslashes($job_id) . "' AND interview.interview_type_id = interview_type.interview_type_id AND
			interview_type.description = '" . SWITCHING . "'";

	$result = $GLOBALS['dbh']->Execute( $sql );
	if (!result) { return T_ERROR; }
	
	if (($result->RecordCount() == 1) && ($row=$result->FetchRow())){
		$isSwitchingInterview = T_TRUE;
	}elseif (($result->RecordCount() > 1)){
		$isSwitchingInterview = T_ERROR;
	}else{
		$isSwitchingInterview = T_FALSE;
	}

	$result->Close();
	
	return $isSwitchingInterview;
}

// }}}
// {{{ function isOffCampusPhone(int $job_id)
/**
 * @return int T_TRUE  - if $job_id is an off-campus phone interview
 *             T_FALSE - if $job_id is either on campus, not a phone interview, or both
 *	           T_ERROR - on failed query or job_id not found
 */
function isOffCampusPhone($job_id)
{

	$sql = "SELECT job_id 
		FROM interview i
        INNER JOIN interview_place ip
          ON i.interview_place_id = ip.interview_place_id
        INNER JOIN interview_medium m
          ON i.interview_medium_id = m.interview_medium_id
		WHERE job_id = '".addslashes($job_id)."' AND
		      ip.description = '" . OFFCAMPUS . "' AND
		      m.description= '" . PHONE . "'";
		      
	$result = $GLOBALS['dbh']->Execute($sql);

	if (!$result) { return T_ERROR; }
	if (($result->RecordCount() == 1))
	{
		$isOffCampusPhone = T_TRUE;
	}
	else
	{
		$isOffCampusPhone = T_FALSE;
	}
	$result->Close();
	
	return $isOffCampusPhone;
}

// }}}
// {{{ function isPhone($interview_medium_id)
/**
 * @return boolean True if the interview_medium_id represents a phone interview
 */
function isPhone($interview_medium_id){
	$sql = "SELECT description
		FROM interview_medium
		WHERE interview_medium_id = $interview_medium_id";
	$result = $GLOBALS['dbh']->Execute($sql);
	if (!$result){ return true; }

	$row = $result->FetchRow();

	if ($row["description"] == PHONE){
		$result->Close();
		return true;
	}else{
		$result->Close();
		return false;
	}
}

// }}}
// {{{ function isVideo($interview_medium_id)
/**
 * @return boolean true if the interview_medium_id represents a video interview
 */
function isVideo($interview_medium_id)
{
	$sql = ("
		SELECT description
		FROM interview_medium
		WHERE interview_medium_id=$interview_medium_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if (!$result)
	{
		return true;
	}
	$row = $result->FetchRow();

	if ($row["description"] == VIDEOCONFERENCE)
	{
		$result->Close();
		return true;
	}
	else
	{
		$result->Close();
		return false;
	}
}

// }}}
// {{{ function isSwitching($interview_type_id)
/**
 * @return boolean True if the interview_type_id represents a switching interview
 */
function isSwitching($interview_type_id){
	$sql = "SELECT description
		FROM interview_type
		WHERE interview_type_id = '" . $interview_type_id . "'";
	$result = $GLOBALS['dbh']->Execute($sql);
	if (!$result){ return false; }

	$row = $result->FetchRow();
	if ($row["description"] == SWITCHING){
		$result->Close();
		return true;
	}else{
		$result->Close();
		return false;
	}
}

// }}}
// {{{ function isSimultaneous($interview_type_id)
/**
 * @return boolean True if the interview_type_id represents a simultaneous interview
 */
function isSimultaneous($interview_type_id){
	$sql = "SELECT description
		FROM interview_type
		WHERE interview_type_id = '" . $interview_type_id . "'";
	$result = $GLOBALS['dbh']->Execute($sql);
	if (!$result){ return false; }

	$row = $result->FetchRow();
	if ($row["description"] == SIMULTANEOUS){
		$result->Close();
		return true;
	}else{
		$result->Close();
		return false;
	}
}

// }}}
// {{{ function isSequential($interview_type_id)
/**
 * @return boolean True if the interview_type_id represents a sequential interview
 */
function isSequential($interview_type_id)
{
	$sql = ("
		SELECT description
		FROM interview_type
		WHERE interview_type_id='" . $interview_type_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if (!$result)
	{
		return false;
	}
	$row = $result->FetchRow();
	$result->Close();
	if ($row["description"] == SEQUENTIAL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// }}}
// {{{ function getIntJobCode($job_id)

/**
 * Get the job code(s) for an interview
 */
function getIntJobCode($job_id)
{
	$sql = ("
		SELECT DISTINCT job_id
		FROM interview_job_join
		WHERE interview_job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if ($result->RecordCount())
	{
		while ($row = $result->FetchRow())
		{
			$job_ids .= ("'" . $row["job_id"] . "',");
		}
		$job_ids = substr($job_ids, 0, -1);
		
		$sql = ("
			SELECT DISTINCT job_code
			FROM job_info
			WHERE job_id IN (" . $job_ids . ")
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		
		while ($row = $result->FetchRow())
		{
			$job_codes[] = $row["job_code"];
		}
		return $job_codes;
	}
	else
	{
		$sql = ("
			SELECT DISTINCT job_code
			FROM job_info
			WHERE job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$job_codes[] = $row["job_code"];
		return $job_codes;
	}
}
// }}}
// {{{ function getDateTime($time_id)
/**
 * @return mixed Array("date", "time") - date and time of $time_id
 *               false - if $time_id is not valid or query fails
 */
function getDateTime($time_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	int_date, int_time
		FROM		interview_time
		WHERE		time_id='" . addslashes($time_id) . "'");

	if (!$result) { return false; }
	if (($result->RecordCount() == 1) && ($row = $result->FetchRow()))
	{
		$getDateTime = Array("date"=>$row["int_date"], "time"=>$row["int_time"]);
	}
	else
	{
		$getDateTime = false;
	}
	$result->Close();
	
	return $getDateTime;
}

// }}}
// {{{ function isUnsignedTimeID($time_id)
/**
 * @return boolean true  - if $time_id is an unsigned interview time
 *                 false - if $time_id is a signed interview time
 */
function isUnsignedTimeID($time_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	student_number, time_id
		FROM		interview_time
		WHERE		time_id='" . addslashes($time_id) . "' AND
					int_type=1");

	if (!$result) { return false; }
	$isUnsignedTimeID = (($result->RecordCount() == 1) && ($row = $result->FetchRow()) && ($row["student_number"] == "") && ($row["time_id"] == $time_id));
	$result->Close();
	
	return $isUnsignedTimeID;
}

// }}}
// {{{ function getTimeID(int $job_id, str $student_number)
/**
 * @return array Array(ternary "return", str "value")
 *   	         return=T_TRUE, value=time_id - time_id for sign-up of student_number for job_id
 *	    		 return=T_FALSE, value="" - student_number is not signed up for job_id
 *				 return=T_ERROR, value=error description - query failed
 */
function getTimeID($job_id, $student_number)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT time_id
		FROM interview_time
		WHERE job_id='" . addslashes($job_id) . "' 	
		AND student_number='" . addslashes($student_number) . "' 
		AND int_type=1 
		AND switch=0
		AND sequential=0
		");

	if (!$result)
	{
		return Array("return"=>T_ERROR, "value"=>"Query failed.");
	}

	$rows = $result->RecordCount();
	if ($rows == 1)
	{
		$row = $result->FetchRow();
		$getTimeID = Array("return"=>T_TRUE, "value"=>$row["time_id"]);
	}
	elseif ($rows == 0)
	{
		$getTimeID = Array("return"=>T_FALSE, "value"=>"");
	}
	else
	{
		$getTimeID = Array("return"=>T_ERROR, "value"=>"Query returned multiple rows.");
	}
	
	$result->Close();
	
	return $getTimeID;
}

// }}}
// {{{ function getSequentialEndTime(int $time_id)
/**
 * @return mixed The actual end time of the given interview (after taking account for sequential interviews) if it is 
 * a sequential interview, and false if it is not.
 */
function getSequentialEndTime($time_id)
{
	$sql = ("
		SELECT DISTINCT a.interview_type_id, b.int_time, b.end_time, a.job_id
		FROM interview AS a, interview_time AS b
		WHERE b.time_id='" . addslashes($time_id) . "'
		AND a.job_id=b.job_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	if (isSequential($row["interview_type_id"]))
        {
        	$stagger_amount = cal_length($row["int_time"], $row["end_time"]);
		$end_time = $row["end_time"];

		$sql = ("
			SELECT DISTINCT sequential
			FROM interview_time
			WHERE job_id='" . $row["job_id"] . "'
			ORDER BY sequential DESC
			");
		$seq_result = $GLOBALS['dbh']->Execute($sql);

		/*
		 We only want the first row from this result, as that will contain the largest value for
		 sequential.
		*/

		$seq_row = $seq_result->FetchRow();
		$seq_result->Close();
		$num_times = $seq_row["sequential"];

		for ($i = 0; $i < $num_times; $i++)
		{
			$end_time = add_times($end_time, $stagger_amount);
		}

		/*
		 We have now set the correct end_time.
		*/

		return $end_time;
	}
	else
	{
		return false;
	}
}

// }}}
// {{{ function get_sequential($time_id)
/**
 * This function takes a time_id, and returns all of the associated sequential time_id's for it.
 */
function get_sequential($time_id)
{                       
        $time_ids = "'" . addslashes($time_id) . "'";
                                                                
        $sql = ("               
                SELECT DISTINCT int_date, int_time, end_time, sequential, job_id
                FROM interview_time
                WHERE time_id='" . addslashes($time_id) . "'
                ");     
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
                
        $stagger_amount = cal_length($row["int_time"], $row["end_time"]);
        $cur_sequential = $row["sequential"];  // This should always start at 0.
        $job_id = $row["job_id"];
        $int_date = $row["int_date"];
        $int_time = $row["int_time"];
        $end_time = $row["end_time"];
                                
        /*                      
         $i only goes up to 6 in the next loop, because there should NEVER be any more than 5 people a student has
         to see sequentially.  This can be changed later if necessary.
        */              
                                
        for ($i = 0; $i < 6; $i++)
        {
                $int_time = add_times($int_time, $stagger_amount);
                $end_time = add_times($end_time, $stagger_amount);
                $cur_sequential++;

                $sql = ("
                        SELECT DISTINCT time_id
                        FROM interview_time
                        WHERE int_date='" . addslashes($int_date) . "'
                        AND int_time='" . addslashes($int_time) . "'
                        AND end_time='" . addslashes($end_time) . "'
                        AND job_id='" . addslashes($job_id) . "'
                        AND sequential='" . addslashes($cur_sequential) . "'
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                if ($result->RecordCount() == 0)
                {
                        /*
                         There are no more time_id's that match correspond, so jump out and return the time_id array.
                        */
                        break;
                }
		$row = $result->FetchRow();
                $time_ids .= ", '" . addslashes($row["time_id"]) . "'";
        }
        return $time_ids;
}

// }}}
// }}}
// {{{ Formatting Functions
// {{{ function make_placeholders($arr) 
function make_placeholders($arr){ return implode(',', array_fill(0, count($arr), '?')); }
// }}}
// {{{ function formatTime($time, $leading_space) 
/** 
 * purpose: formats a time (as they are pulled from the database) for display.
 *   setting the optional second parameter to false supresses the leading space
 * @param string $time The time to format, in HH:MM:SS format.
 * @param boolean $leading_space True if you want a leading space on the output.
 * @return string The formatted time.
 */
function formatTime($time, $leading_space = true)
{
   $splittime = explode(":", $time);
   $time = date("g:i a", mktime($splittime[0], $splittime[1], 0));
   if ((strlen($time) < 8) && $leading_space) { $time = "&nbsp;" . $time; }
   return $time;
}
// }}}
// {{{ function txtFormatTime($time)
/**
 * purpose: formats a time (as they are pulled from the database) for display in
 *     monospace text (eg. e-mail).
 * @param string $time The time from the DB in HH:MM:SS format.
 * @return string The formatted time.
 */
function txtFormatTime($time)
{
	$time = formatTime($time, false);
   if ((strlen($time) < 8)) { $time = " " . $time; }
   return $time;
}
// }}}
// {{{ function formatLongDate($date)
/**
 * Formats a date (as they are pulled from the database) for display in long format.
 * @param string $date The date to format.
 * @return string The formatted date.
 */
function formatLongDate($date)
{
   $splitdate = split("-", $date, 3);
   return date('l, F j, Y', mktime(0, 0, 0, $splitdate[1], $splitdate[2], $splitdate[0]));
}
// }}}
// {{{ function formatStandardDate($date)
/** 
 * Purpose: formats a date (as they are pulled from the database) for display in long format but with
 *          Abbreviated days and months.
 * @param string $date The date to format in YY-DD-MM.
 * @return string The formatted date.
 */ 
function formatStandardDate($date)
{
   $splitdate = split("-", $date, 3);
   return date('D, M. j, Y', mktime(0, 0, 0, $splitdate[1], $splitdate[2], $splitdate[0]));
}
// }}}
// {{{ function formatShortDate($date)
/** 
 * purpose: formats a date (as they are pulled from the database) for display in short format
 *          eg. Jan. 14, 2004
 * @param string $date The date to format, in YY-DD-MM format.
 * @return string The formatted date.
 */
function formatShortDate($date)
{
   $splitdate = split("-", $date, 3);
   return date('M. j, Y', mktime(0, 0, 0, $splitdate[1], $splitdate[2], $splitdate[0]));
}

// }}}
// {{{ function formatSpecialZeroDate($date)
/**
 * purpose: It takes a date of the format YYYY-MM-DD, and converts it into somethING LIKE jANUARY 14, 2004
 *          except, we may have the input 2004-11-00, and it'll convert the date into January, 2004
 * @param string $date The date from the datebase.
 * @return string The formatted date.
 */
function formatSpecialZeroDate($date)
{
   $splitdate = split("-", $date, 3);
   if ($splitdate[2] == "00")
   {
        return getMonth($splitdate[1]).", ".$splitdate[0];
   }
   else
   {
        return date("F j, Y", mktime(0, 0, 0, $splitdate[1], $splitdate[2], $splitdate[0]));
   }
}

// }}}
// {{{ function strip_html_tags($str)

/**
 * Description:
 *   returns a string with all <h1> tages removes and all <html> and <head> and <script> tags removed (just the html
 *      between the <body> tags will be left)
 *   converts <h1> to <h3>
 *            <h2> to <h4>
 *
 */
function strip_html_tags($str)
{

	$pattern = array ("/<H1[^>]*>/i", "/<\/h1>/i", "/<H2[^>]*>/i", "/<\/h2>/i", "/<HTML[^>]*>/i", "/<script[^>]*>/i",
				"/<\/SCRIPT[^>]*>/i", "/<HEAD.*>/i", "/<BODY[^>]*>/i", "/<\/html>/i", "/<\/head>/i", "/<\/body>/i", 
				"/<TITLE[^>]*>.*<\/title>/i", "/<STYLE[^>]*>.*<\/style>/i", "/<PRE[^>]*>/i", "/<\/PRE[^>]*>/i",
				"/<\?/i", "/\?>/i", "/<FORM[^>]*>/i", "/<\/FORM[^>]*>/i", "/<INPUT[^>]*>/i");
	$replace = array ("<h3>", "</h3>", "<h4>", "</h4>");

	$str_new = preg_replace($pattern, $replace, $str);
	
	return $str_new;

}
// }}}
// {{{ function print_array($src_array)

function print_array($src_array)
{
        assert( is_array($src_array) );
        echo("<hr />Print Array<br />");
        foreach( $src_array as $key=>$value )
        {
                echo( "Key: ". $key. "  Value:" . $value . "<br />");
        }
}
// }}}
// {{{ function normalizeWebsite($website)
/**
 * Function below normalizes website by removing http:// from in front of it if there is one.
 */
function normalizeWebsite($website)
{
        if ( trim($website) == '')
        {
                return $website;
        }
        if (ereg("http://.*$", $website))
        {
                $website = ereg_replace("^(http://)(.*)$", "\\2", $website);
        }

        return $website;
}

// }}}
// {{{ function notify($text)
/**
 * purpose:	displays a notification in a consistent manner
 * @return void
 */
function notify($text)
{
	?>
		<table width="100%" class="notify" cellpadding="5" cellspacing="0" border="1">
		<tr><td><?php echo($text); ?></td></tr>
		</table>
	<?php
}

// }}}
// {{{ function error($text)
/**
 * purpose:	displays error messages in a consistent manner
 * @return void
 */
function error($text)
{
	?>
		<table width="100%" class="error" cellpadding="5" cellspacing="0" border="1">
		<tr><td><?php echo($text); ?></td></tr>
		</table>
	<?php
}

// }}}
// {{{ function instruct($text)
/**
 * purpose: displays instructions in a consistent manner
 * @return void
 */
function instruct($text)
{
	?>
		<table width='100%' class='row1' cellpadding='8' cellspacing='0' border='4'>
		<tr><td><b class='black'><?php echo($text); ?></b></td></tr>
		</table>
	<?php
}

// }}}
// {{{ function info($text)
/**
 * purpose: displays information in a consistent manner
 * @return void
 */
function info($text)
{
	echo("<table width='100%' class='backgrey' cellpadding='5' cellspacing='0' border='1'>");
	echo("<tr><td>");
	echo($text);
	echo("</td></tr>");
	echo("</table>");
}

// }}}
// {{{ function unslash($string)
/**
 * This is meant to be used to get rid of slashes added by the web browser
 * when forms are submitted (all browsers I checked seemed to do this although
 * it doesn't seem to be in agreement with IETF's RFC 1738)
 * @return string Returns a string with the following translations \\ -> \, \' -> ', \" -> "
 */
function unslash($string)
{
	// Why is the regexp not "/\\([\\'\"])/" ?  I don't know; however, it doesn't work.
	return preg_replace("/\\\\([\\\\'\"])/", "\\1", $string);
}

// }}}
// {{{ function clock()
/**
 * @return string HTML clock code
 */
function clock()
{
	$timestamp = time();
	$output = "<div align='center' class='clock'>";
	$output .= date("D M j, Y", $timestamp) . "<br />";
	$output .= "Refreshed at " . date("g:i:s A", $timestamp);
	$output .= "</div>";
	
	return $output;
}

// }}}
// {{{ function DBG_GetBacktrace()
/**
 * Returns a nicely formatted backtrace.
 * @param int $skip Optional, the number of calls to skip (if beinc called from some other function, 
 *                  such as notifyDeprecated, a value of 1 would skip reporting the notifyDeprecated call.
 * @return string Nice backtrace output.
 * @author (from PHP.net) spagmoid@yahoo.com
 * @see debug_backtrace()
 */
function DBG_GetBacktrace($skip = 0)
{
    $traceArr = debug_backtrace();
    $s = '';
    $MAXSTRLEN = 64;

    $s = '<pre align=left>';
    for($i = 0; $i<=$skip; $i++) array_shift($traceArr);
    $tabs = sizeof($traceArr)-1;
    foreach($traceArr as $arr)
    {
        for ($i=0; $i < $tabs; $i++) $s .= ' &nbsp; ';
        $tabs -= 1;
        $s .= '<font face="Courier New,Courier">';
        if (isset($arr['class'])) $s .= $arr['class'].'.';
        $args = array();
        if(!empty($arr['args'])) foreach($arr['args'] as $v)
        {
            if (is_null($v)) $args[] = 'null';
            else if (is_array($v)) $args[] = 'Array['.sizeof($v).']';
            else if (is_object($v)) $args[] = 'Object:'.get_class($v);
            else if (is_bool($v)) $args[] = $v ? 'true' : 'false';
            else
            { 
                $v = (string) @$v;
                $str = htmlspecialchars(substr($v,0,$MAXSTRLEN));
                if (strlen($v) > $MAXSTRLEN) $str .= '...';
                $args[] = "\"".$str."\"";
            }
        }
        $s .= $arr['function'].'('.implode(', ',$args).')</font>';
        $Line = (isset($arr['line'])? $arr['line'] : "unknown");
        $File = (isset($arr['file'])? $arr['file'] : "unknown");
        $s .= sprintf("<font color=#808080 size=-1> # line %4d, file: <a href=\"%s\">%s</a></font>",
                $Line, $File, $File);
        $s .= "\n";
    }    
    $s .= '</pre>';
    return $s;
}
// }}}
// {{{ function notifyDeprecated($function)
/**
 * Returns a nicely formatted backtrace with a deprecation notice.
 * @param string $function The name of the function you want to report as deprecated.
 *                          i.e.  Class::functionName()  or just functionName() if not a method.
 * @return string Nice deprecation message.
 * @see DBG_GetBacktrace()
 */
function notifyDeprecated($function)
{
    notify("Deprecated function $function called.<br />" . DBG_GetBacktrace(1));
}
// }}}
// {{{ function makeStringOverlibSafe($string)
/**
 * @param string $function The string to be displayed in an overlib hover box 
 * @param int $mode When outputting an overlib string, all of the quotes have to be one type, or else it interferes with the javascript string that is 
 *                  being outputted. So by default, we will take all double quotes and turn them into single quotes. If $mode is set, then we take all
 *                  single quotes and turn them into double quotes. The mode you select depends on how you're making thing overlib string. 
 *                  Example: echo("<span onmouseover=\"return overlib('".makeStringOverlibSafe($string)."');\">"); would require the default mode.
 * @return string A string that can now be used safely with overlib 
 */
function makeStringOverlibSafe($string, $mode = 0)
{
    if ($mode) {
        $string = str_replace('"', '\"', $string);
        $string = str_replace('\'', '\"', $string);
    }
    else {
        $string = str_replace("'", "\'", $string);
        $string = str_replace("\"", "\'", $string);
    }
    $string = str_replace("\r\n","<br />",$string);
    return $string;
}
// }}}
// {{{ function normalizeSalary($salary)
/**
 * Normalizes the salary given by adding either .00, 00, or 0 to the amount passed in and returning it.
 * @param double or int $salary A salary value. 
 * @returns:	double or int - which is the new normalized salary 
 *				false - if $salary is empty
 */
function normalizeSalary($salary)
{
	if ( trim($salary) == '')
	{
		return false;
	}
	
	if (!isValidSalary($salary))
	{
		return $salary;
	}

	if (ereg('^[0-9]+\.$', (trim($salary))))
        {       
                $salary = $salary . "00";
        } elseif (ereg('^[0-9]+$', (trim($salary))))
        {       
                $salary = $salary . ".00";
        } elseif (ereg('^[0-9]+\.[0-9]{1}$', (trim($salary))))
        {       
                $salary = $salary . "0";
        }
	
	return $salary;
}
// }}}
// }}}

// {{{ Validating Functions
// {{{ function isValidCompanyName(str $company_name)
/**
 * @return boolean true  - if $company_name is a valid company_name
 *                 false - if $company_name not a valid company_name
 */
function isValidCompanyName($company_name, $department_str = "")
{
	if (trim($company_name) == "") { return false; }
	if ($department_str == ""){
		$result = $GLOBALS['dbh']->Execute("
			SELECT company_name
			FROM employer_company
			WHERE company_name = '" . addslashes(stripslashes($company_name)) . "'");
	}else{
		$sql = ("
			SELECT	c.company_name
			FROM	employer_company as c, job_info as j
			WHERE	c.employer_id = j.employer_id AND j.department_id IN (" . $department_str . ") AND c.company_name='" . addslashes($company_name) . "'");
		$result = $GLOBALS['dbh']->Execute($sql);
	}
	if (!$result) { return false; }
	$isValidCompanyName = ($result->RecordCount() >= 1);
	$result->Close();
	
	return $isValidCompanyName;
}

// }}}
// {{{ function isValidEmployerID(int $employer_id)
/**
 * @return boolean true  - if $employer_id is a valid employer_id
 *				   false - if $employer_id not a valid employer_id
 */
function isValidEmployerID($employer_id, $department_str = "")
{	
	if (trim($employer_id) == "") { return false; }
	
	if ($department_str == ""){
		$result = $GLOBALS['dbh']->Execute("
			SELECT	company_name
			FROM		employer_company
			WHERE		employer_id='" . addslashes($employer_id) . "'");
	}else{
		$sql = ("
			SELECT e.company_name
			FROM employer_company as e, job_info as j
			WHERE e.employer_id = j.employer_id AND e.employer_id = '" . addslashes($employer_id) . "' AND j.department_id IN (" . $department_str . ")");
		$result = $GLOBALS['dbh']->Execute($sql);
	}
	if (!$result) { return false; }
	$isValidEmployerID = ($result->RecordCount() >= 1);
	$result->Close();
	
	return $isValidEmployerID;
}

// }}}
// {{{ function isValidStudentNumber(str $student_num)
/**
 * @return boolean true  - if $student_num is a valid student_number
 *				   false - if $student_num not a valid student_number
 */
function isValidStudentNumber($student_num, $department_in_str = "")
{	
	if (trim($student_num) == "") 
    { 
        return false; 
    }
	if ($department_in_str == "")
    {
		$sql = ("
			SELECT	student_number
			FROM		student
			WHERE		student_number='" . $student_num . "'");
        $result = $GLOBALS['dbh']->Execute($sql);
	}
    else
    {
		$sql = ("
			SELECT s.student_number
			FROM student as s, student_department as sd
			WHERE s.student_number = '" . $student_num . "' AND
				sd.department_id IN (" . $department_in_str . ") and sd.student_number=s.student_number");
        $result = $GLOBALS['dbh']->Execute($sql);
	}
	if (!$result) 
    { 
        return false; 
    }
    else{	
        
        $row=$result->FetchRow();
        $isValidStudentNumber=$row['student_number'];
        $result->Close();
        return $isValidStudentNumber;
        
    }
}

// }}}
// {{{ function isValidStudentName(str $student_name)
/**
 * @return boolean true  - if $student_name is a valid student_name
 *				   false - if $student_name not a valid student_name
 */
function isValidStudentName($student_name, $department_in_str)
{	
    $student_name = trim($student_name);
    if($student_name == "") return false;

    if (preg_match("/,.*,/", $student_name))
    {
        $error = 1;
        error("You may only enter one comma in the student name field.");
    }
    elseif ($student_name == ",")
    {                                                                                                                                        
        $error = 1;
        error("You must include either a first name or last name to edit a student based on their name.");
    }
    else
    {
        // Strip wild card character from student name. 
        $student_name = str_replace("%", "", $student_name);
        $student_names = explode(",", $student_name);
        $first_name = trim($student_names[1]);
        $last_name = trim($student_names[0]);
        
        $sql = ("
                SELECT s.student_number
                FROM student AS s
                INNER JOIN student_department AS sd ON sd.student_number=s.student_number
                WHERE sd.department_id IN (" . $department_in_str . ")
                ");

        if ($first_name)                                                                                                                     
        {                                                                                                                                    
            $sql .= ("                                                                                                                       
                    AND s.first_name LIKE '" . addslashes($first_name) . "%'                                                                                 
                    ");                                                                                                                          
        }                                                                                                                                    

        if ($last_name)                                                                                                                      
        {                                                                                                                                    
            $sql .= ("                                                                                                                       
                    AND s.last_name LIKE '" . addslashes($last_name) . "%'                                                                                   
                    ");                                                                                                                          
        }
        
        $result = $GLOBALS['dbh']->Execute($sql);
	}
    
    if (!$result) 
    { 
        return false; 
    }
    else
    {
        $isValidStudentName = ($result->RecordCount());
        $result->Close();
        return $isValidStudentName;
    }
}

// }}}
// {{{ function isValidDate(str $sdate)
/**
 * @return boolean true  - if $sdate valid yyyy-mm-dd
 *                 false - if $sdate not valid yyyy-mm-dd
 */
function isValidDate($sdate)
{
   // strip trailing and leading spaces
   $sdate = trim($sdate);

   if (!preg_match("/^\d{4,4}-\d{2,2}-\d{2,2}$/", $sdate)) { return false; }

	// date already has a separator
   $datearray = explode("-", $sdate);

   $elements = sizeof($datearray);
   if ($elements == 3)
   {
      $y = $datearray[0];
      $m = $datearray[1];
      $d = $datearray[2];
   }
   else
   {
		return false;
   }

   if ($y < 1000 || $y > 9999 || $m < 1 || $m > 12 || $d < 1 || $d > 31) { return false; }
   if (($m == 4 || $m == 6 || $m == 9 || $m == 11) && $d == 31) { return false; }
   if ($m == 2 && $d > 28)
   {
      if ($d > 29) { return false; }
      if (!(($y % 4 == 0) && (($y % 100 != 0) || ($y % 400 == 0)))) { return false; }
   }

   return true;
}

// }}}
// {{{ function isValidDate2(str $sdate)
/**
 * Same as isValidDate except, we accept dates that are 00, ie 2004-04-00. 
 * @return boolean true  - if $sdate valid yyyy-mm-dd
 *                 false - if $sdate not valid yyyy-mm-dd
 */
function isValidDate2($sdate)
{
   // strip trailing and leading spaces
   $sdate = trim($sdate);

   if (!preg_match("/^\d{4,4}-\d{2,2}-\d{2,2}$/", $sdate)) { return false; }

	// date already has a separator
   $datearray = explode("-", $sdate);

   $elements = sizeof($datearray);
   if ($elements == 3)
   {
      $y = $datearray[0];
      $m = $datearray[1];
      $d = $datearray[2];
   }
   else
   {
		return false;
   }

   if ($y < 1000 || $y > 9999 || $m < 1 || $m > 12 || $d < 0 || $d > 31) { return false; }
   if (($m == 4 || $m == 6 || $m == 9 || $m == 11) && $d == 31) { return false; }
   if ($m == 2 && $d > 28)
   {
      if ($d > 29) { return false; }
      if (!(($y % 4 == 0) && (($y % 100 != 0) || ($y % 400 == 0)))) { return false; }
   }

   return true;
}

// }}}
// {{{ function isValidJobID_spec($job_id, $department_str = "") 
/**
 * This function is used to validate a job_id with a departments specialties.
 * @return boolean Returns true if valid and false otherwise.
 */
function isValidJobID_spec($job_id, $department_str = "") 
{
	if ($department_str == "") {
		$result = $GLOBALS['dbh']->Execute("SELECT job_id FROM job_info WHERE job_id='" . addslashes($job_id) . "'");
	}
	else 
	{
		$select_specialties = $GLOBALS['dbh']->Execute("
			SELECT specialties 
			FROM job_info 
			WHERE job_id='" . addslashes($job_id). "'
			");

		if ($specialty_row = $select_specialties->FetchRow()) 
		{
			$array_specialties = explode(",", $specialty_row["specialties"]);
			$check_depspec = $GLOBALS['dbh']->Execute("SELECT specialty_id from department_specialties where department_id IN ($department_str)");
				
			$found = 0;
			while (!$found AND $checkrow = $check_depspec->FetchRow()) 
			{
				$i = 0;
				while ($i < sizeof($array_specialties) ) 
				{
					if ($checkrow["specialty_id"] == $array_specialties[$i]) 
					{
						$found = 1;
					}
					$i++;
				}
			}
			if ($found) 
			{
				$result = $GLOBALS['dbh']->Execute("
					SELECT job_id 
					FROM job_info 
					WHERE job_id='" . addslashes($job_id ) . "'
					");
			} 
			else 
			{
				return false;
			}
		} 
		else 
		{ 
			return false; 
		}
	


	//	$result = $GLOBALS['dbh']->Execute("SELECT job_id FROM job_info
	//				WHERE department_id IN (" . $department_str . ")
	//				AND job_id='" . addslashes($job_id) . "'");

	}
	if (!$result) { return false; }

	$isValidJobID = (($result->RecordCount() == 1) && ($row = $result->FetchRow()) && ($row["job_id"] == $job_id));
	$result->Close();


	return $isValidJobID;
}


// }}}
// {{{ function isValidJobCode($job_code, $department_str)
/**
 * @returns:	true  - if $job_code is a valid job_code
 *				false - if $job_code not a valid job_code
 */
function isValidJobCode($job_code, $department_str)
{
	
	if ($department_str == ""){
		$result = $GLOBALS['dbh']->Execute("SELECT job_code FROM job_info WHERE job_code='" . addslashes($job_code) . "'");
	}else{
		$result = $GLOBALS['dbh']->Execute("SELECT job_code FROM job_info 
					WHERE department_id IN (" . $department_str . ")
					AND job_code='" . addslashes($job_code) . "'");
	}
	
	if (!$result) { return false; }
	$isValidJobCode = ($result->RecordCount() >= 1);
	$result->Close();
	
	return $isValidJobCode;
}

// }}}
// {{{ function isValidTimeID($time_id, $job_id=false, $interviews_only=true, $no_switch=false)
/**
 * @param int $job_id The optional job_id parameter also checks to make sure the time_id corresponds to job_id
 * @return boolean true  - if $time_id is a valid time_id
 *				   false - if $time_id not a valid time_id
 */
function isValidTimeID($time_id, $job_id=false, $interviews_only=true, $no_switch=false)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT	time_id
		FROM		interview_time
		WHERE		time_id='" . addslashes($time_id) . "'
		" . (($interviews_only) ? "AND int_type=1" : "" ) . "
		" . (($job_id) ? "AND job_id='" . addslashes($job_id) . "'" : "") . "
		" . (($no_switch) ? "AND NOT switch" : ""));
		
	if (!$result) { return false; }
	$isValidTimeID = $result->RecordCount();
	$result->Close();
	
	return $isValidTimeID;
}

// }}}
// {{{ function validDate($sdate)

/**
 * purpose: checks whether a date is valid.  Returns the date formatted as 'yyyy-mm-dd' on a valid date,
 *          -1 on an invalid date.
 * accepted input forms:  (double letters signify leading 0 to pad to two-digits)
 *    yyyy-mm-dd, yyyy-mm-d, yyyy-m-dd, yyyy-m-d, yy-mm-dd, yy-mm-d, yy-m-dd, yy-m-d, mm-dd, mm-d, m-dd, m-d
 *    yyyy/mm/dd, yyyy/mm/d, yyyy/m/dd, yyyy/m/d, yy/mm/dd, yy/mm/d, yy/m/dd, yy/m/d, mm/dd, mm/d, m/dd, m/d
 *    yyyymmdd, yymmdd, mmdd
 * notes: if the year is not specified, the current year is assumed.  If the century is not specified
 *        the current century is assumed
 *
 * acceptable range: year 1000 - 9999
 */

function validDate($sdate)
{

   // strip trailing and leading spaces
   $sdate = trim($sdate);

   if (!preg_match("/^((\d\d){0,1}\d\d[-\/]){0,1}\d{1,2}[-\/]\d{1,2}$/", $sdate) &&
       !preg_match("/^((\d\d){0,1}\d\d){0,1}\d{4,4}$/", $sdate)) { return -1; }

   // check if the date already has a date separator (i.e. '-' or '/')
   if (!preg_match("/[-\/]/", $sdate))
   {
      // date doesn't have a separator
      $length = strlen($sdate);

      if ($length == 4)
      {
         // mmdd
         $y = date("Y");   // use current year
         $m = substr($sdate, 0, 2);
         $d = substr($sdate, 2, 2);
      }
      elseif ($length == 6)
      {
         // yymmdd
         $y = date("Y");   // get current year
         $y -= $y % 100;    // strip off everything but the century
         $y += substr($sdate, 0, 2);   // add the two digit year to the century
         $m = substr($sdate, 2, 2);
         $d = substr($sdate, 4, 2);
      }
      elseif ($length == 8)
      {
         // yyyymmdd
         $y = substr($sdate, 0, 4);
         $m = substr($sdate, 4, 2);
         $d = substr($sdate, 6, 2);
      }
      else
      {
         return -1;
      }
   }
   else
   {
      // date already has a separator
      // change any '/' to '-'
      $sdate = preg_replace("/\//", "-", $sdate);
	$datearray = explode("-", $sdate);

      $elements = sizeof($datearray);
      if ($elements == 3)
      {
         $y = $datearray[0];
         if ($y < 100)
         {
            // two digit year
            $ytemp = date("Y");   // get current year
            $y += $ytemp - ($ytemp % 100);    // strip off everything but the century and add to two-digit year
         }
         $m = $datearray[1];
         $d = $datearray[2];
      }
      elseif ($elements == 2)
      {
         $y = date("Y");   // no year given, use current year
         $m = $datearray[0];
         $d = $datearray[1];
      }
      else
      {
         return -1;
      }
   }

   if ($y < 1000 || $y > 9999 || $m < 1 || $m > 12 || $d < 1 || $d > 31) { return -1; }
   if (($m == 4 || $m == 6 || $m == 9 || $m == 11) && $d == 31) { return -1; }
   if ($m == 2 && $d > 28)
   {
      if ($d > 29) { return -1; }
      if (!(($y % 4 == 0) && (($y % 100 != 0) || ($y % 400 == 0)))) { return -1; }
   }

/*
   // a bug in PHP prevents the last parameter from being recognized.
   $m = str_pad($m, 2, "0", STR_PAD_LEFT);
   $d = str_pad($d, 1, "0", STR_PAD_LEFT);
*/
   // work-around for aforementioned bug
   $m = substr("0" . $m, -2);
   $d = substr("0" . $d, -2);


   $sdate = $y . "-" . $m . "-" . $d;

   return $sdate;
}
// }}}
// {{{ function validTime($stime)

/**
 * purpose: Validates a time.  The function accepts a string and trims any leading or
 *         trailing spaces.  See below for inputs and outputs
 * notes: Note like ISO8601 time, there is no 24:00
 * Input general:  colon is optional (that is '12:30' and '1230' are equivalent), case of am/pm is ignored
 * 24 hr input
 * input:      0:00 -  9:59     00:00 - 09:59     10:00 - 19:59     20:00 - 23:59
 * output:     0:00 -  9:59     00:00 - 09:59     10:00 - 19:59     20:00 - 23:59
 * 12 hr input
 * input  a:   1:00AM -  9:59AM      1:00A  -  9:59A      10:00AM - 11:59AM     10:00A  - 11:59A
 * output a:   1:00   -  9:59        1:00   -  9:59       10:00   - 11:59       10:00   - 11:59
 * input  b:  01:00AM - 09:59AM     01:00A  - 09:59A      12:00AM - 12:59AM     12:00A  - 12:59A
 * output b:  01:00   - 09:59       01:00   - 09:59       00:00   - 00:59       00:00   - 00:59
 * input  c:  12:00PM - 12:59PM     12:00P  - 12:59P
 * output c:  12:00   - 12:59       12:00   - 12:59
 * input  d:   1:00PM -  9:59PM      1:00P  -  9:59P      10:00PM - 11:59PM     10:00P  - 11:59P
 * output d:  13:00   - 21:59       13:00   - 21:59       22:00   - 23:59       22:00   - 23:59
 * input  e:  01:00PM - 09:59PM     01:00P  - 09:59P
 * output e:  13:00   - 21:59       13:00   - 21:59
 * 
 * input:   anything else
 * output:  -1
 */ 

function validTime($stime)
{
   // strip trailing and leading spaces
   $stime = trim($stime);

   // make sure the time has a ":", if it doesn't, add it.
   $stime = preg_replace("/^([^:]*)(\d{2}[^\d]*)$/", "\\1:\\2", $stime);

   if (preg_match ("/^(([0-1]{0,1}[0-9])|(2[0-3])):[0-5][0-9]$/", $stime))
   {
      // matches 0:00-9:59, 00:00-09:59, 10:00-19:59, 20:00-23:59
      // do nothing, already in acceptable format.
   }
   elseif (preg_match ("/^((0{0,1}[1-9])|(1[0-2])):[0-5][0-9] {0,1}[aApP][mM]{0,1}$/", $stime))
   {
      // matches 1:00-9:59, 01:00-09:59, 10:00-12:59, followed by suffix '( )a|A|p|P(m|M)'
      $am = (preg_match ("/[aA]/", $stime)) ? TRUE : FALSE;
      // strip am or pm
      $stime = preg_replace("/ {0,1}[aApP][mM]{0,1}/", "", $stime);

      // convert from 12 hour to 24 hour time
      list ($hour, $minute) = split (":", $stime);
      if ($am && ($hour == 12)) {
         $stime = "00" . ":" . $minute;
      }
      elseif (!$am && ($hour != 12)) {
         $stime = ($hour + 12) . ":" . $minute;
      }
      else {
         $stime = $hour . ":" . $minute;
      }
   }
   else
   {
      $stime = -1;
   }

   return $stime;
}
// }}}
// {{{ function check_date(&$date)

/**
 *  Description: checking format of interview date.
 *                               1. if input is invalid return 0 and make no change to the original input date string.
 *                               2. if input is in correct format but earlier than current date, return 0 and leave the original input unchanged
 *                               3. otherwise, return 1
 *
 * @author Entered by Robin, 2000-09-20 9:49
 */
function check_date( &$date )
{
        global $current_date;

        assert( isset($current_date) );

        $temp_date = $date;
        $temp_date = trim($temp_date);

        if( $temp_date == '' ) { return 0; }

        if( ($temp_date=validDate($temp_date)) == -1 ) { return 0; }
        elseif( $temp_date < $current_date )
        {
                return 0;
        }

        $date = $temp_date;
        return 1;
}
// }}}
// {{{ function isValidNetlink($netlink_id, $number)

/**
 * Function below checks with the Netlink server to ensure that the Netlink ID is valid.
 */
function isValidNetlink($netlink_id, $number)
{
    if (SCRIPT_LOGIN == 'netlink'){
        $string_code = "R:".$netlink_id;
            $validator_info[]="";
            exec(SCRIPT_PATH."/client.pl $string_code", $validator_info);
            list ($return_code, $return_type, $return_number, $return_status) = split(":", $validator_info[1]);
    //	echo("code=".$return_code." type=".$return_type." number=".$return_number." status=".$return_status."<br />");
    //	echo("netlink=".$netlink_id." number=".$number."<br />");
            if ($return_code == '1' && $return_number == $number){
            return true;
            } else {
            return false;
        }
    } else {
        return false;
    }
}
// }}}
// {{{ function toSpecialChars($str)

/**
 * This function converts the special characters in the string to its readable form
 */
function toSpecialChars($str)
{
    $specialStr = htmlspecialchars(stripslashes($str),ENT_QUOTES);
    return $specialStr;
}
// }}}
// {{{ function isValidPhoneNumber($phone)

/**
 * Function below checks to make sure that the given phone number is a valid one, in that it has more than 7 digits.
 */
function isValidPhoneNumber($phone)
{
    if (trim($phone) == '')
    {
        return false;
    }

    $isValidPhoneNumber = ereg('(.*[0-9].*){8,}',(trim($phone)));

    return $isValidPhoneNumber;
}
// }}}
// {{{ function isValidTermID($term_id) 

/**
 * function that checks if the provided term id is a valid one. 
 */
function isValidTermID($term_id) 
{
    $sql = ("
        SELECT term_id
        FROM term
        WHERE term_id = '".addslashes($term_id)."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    return $row['term_id'];
}
// }}}
// {{{ function isValidJobID($job_id, $department_str = "")
/**
 * @param int $job_id The job ID.
 * @returns:	true  - if $job_id is a valid job_id
 *				false - if $job_id not a valid job_id
 */
function isValidJobID($job_id, $department_str = "")
{
	if ($department_str == "")
	{
		$sql = ("
			SELECT job_id 
			FROM job_info 
			WHERE job_id='" . addslashes($job_id) . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	}
	else
	{
		$sql = ("
			SELECT job_id 
			FROM job_info 
			WHERE department_id IN (" . $department_str . ") 
			AND job_id='" . addslashes($job_id) . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	}
	if (!$result) { return false; }
	$isValidJobID = (($result->RecordCount() == 1) && ($row = $result->FetchRow()) && ($row["job_id"] == $job_id));
	$result->Close();
	
	return $isValidJobID;
}

// }}}
// {{{ function isValidEmail($address_list)
/**
 * @param string $address_list A common delimited list of e-mail addresses 
 * @returns:	true  - if $address_list contains all valid e-mail addresses 
 *				false - if $address_list contains an invalid e-mail address or the list is empty
 */
function isValidEmail($address_list)
{
    $addresses = preg_split("/\s*,\s*/", $address_list);
    $num_addresses = count($addresses);
    if ($num_addresses == 0) { return false; }
    for ($i = 0; $i < $num_addresses; $i++)
    {
        if (!_isValidAddress(trim($addresses[$i]))) {
            return false;
        }
    }
    return true;
}
// }}}
// {{{ private function _isValidAddress($address)
/**
 * This function is used by isValidEmail to check if individual e-mail addresses are valid. Please use the isValidEmail
 * function as this function is private.  
 * @param string $address A single e-mail address
 * @returns:	true  - if $address is a valid e-mail address
 *				false - if $address is an invalid e-mail address
 */
function _isValidAddress($address)
{
	$emailPat = "/^(.+)@(.+)$/";
	$specialChars = "\(\)<>@,;:\\\"\.\[\]";
	$validChars = "[^\s" . $specialChars . "]";
	$quotedUser = "(\"[^\"]*\")";
	$ipDomainPat = "/^\[(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})\]$/";
	$atom = $validChars . "+";
	$word = "(" . $atom . "|" . $quotedUser . ")";

	$userPat = "/^" . $word . "(\." . $word . ")*$/";
	$domainPat = "/^" . $atom . "(\." . $atom . ")*$/";

	preg_match($emailPat, $address, $matchArray);
	if (count($matchArray) != 3)
	{
		// too many, too few '@'s.  Consecutive '.'s
		return false;
	}

	$user = $matchArray[1];
	$domain = $matchArray[2];

	if (!preg_match($userPat, $user))
	{
	// user is not valid
		return false;
	}

	preg_match($ipDomainPat, $domain, $IPArray);
	if (count($IPArray) != 0)
	{
		// this is an IP address
		for ($i = 1; $i <= 4; $i++)
		{
			if ($IPArray[$i] > 255)
			{
				// invalid IP domain
				return false;
			}
		}
		return true;
	}

	preg_match($domainPat, $domain, $domainArray);
	if (count($domainArray) == 0)
	{
		// domain name not valid
		return false;
	}

	$atomPat = "/$atom/";
	preg_match_all($atomPat, $domain, $domArr);
	$domArr2 = $domArr[count($domArr) - 1];
	$len = count($domArr2);

	$domArr3 = $domArr2[count($domArr2) - 1];
	$last_len = strlen($domArr3);
	if ($last_len < 2 || $last_len > 3)
	{
		// the address must end in a two letter (country) or three letter (domain) word.
	return false;
	}
	if ($len < 2)
	{
		// address missing hostname
		return false;
	}
	return true;
}
// }}}
// {{{ function isValidSalary($salary)
/**
 * @param double $salary A salary value. 
 * @returns:	true  - if $salary is a valid salary 
 *				false - if $salary is not a valid salary
 */
function isValidSalary($salary)
{
	$isValidSalary = ereg('^[0-9]+\.?[0-9]{0,2}$',$salary);
	return $isValidSalary;
}
// }}}
// }}}

// {{{ Date / Time functions
// {{{ function add_dates($date, $num)

/**
 *  Description: add a given number of days to an given date and return the new date
 *
 * @author Entered by Robin, 2000-09-20 9:49
 */
function add_dates($date, $num)
{
   $splittime = explode("-", $date);
   $year = intval( $splittime[0] );
   $month = intval( $splittime[1] );
   $day = intval( $splittime[2] );

   if( checkdate($month, $day, $year) )
   {
      if( !is_int($num) ) $num = intval($num);

      if( checkdate($month, $day+$num, $year) ){ $day = $day+$num; }
      elseif( checkdate($month, 31, $year) ){ $month = $month+1; $day=$num-(31-$day);}
      elseif( checkdate($month, 30, $year) ){ $month = $month+1; $day=$num-(30-$day);}
      elseif( checkdate($month, 28, $year) ){ $month = $month+1; $day=$num-(28-$day);}
      elseif( checkdate($month, 27, $year) ){ $month = $month+1; $day=$num-(27-$day);}

      if($month == 13)
      {
         $month=1;
                 $year=$year+1;
      }
   }
   $year = strval($year);

   if($month < 10) { $month = "0".strval($month); }
   else { $month = strval($month); }
   if($day < 10) { $day = "0".strval($day); }
   else { $day = strval($day); }

   $date = $year . "-" . $month . "-" . $day;
   return $date;
}
// }}}
// {{{ function add_times($time, $int_length)

/**
 *  Description: add a given number of minutes to an interview time
 */
function add_times($time, $int_length)
{
   $splittime = explode(":", $time);

   $min = intval($splittime[1]);
   $hour = intval($splittime[0]);
   $int_length = intval($int_length);

   $min = $min+$int_length;

   while ($min >= 60)
   {
      $hour++;
      $min = $min - 60;
   }
   while( $hour >= 24 )
   {
          $hour = $hour -24;
   }
   $time = date( "H:i", mktime( $hour, $min ) );
   $time = $time . ":00";
   return $time;
}
// }}}
// {{{ function sub_times($time, $int_length)

/**
 *  Description: subtract a given number of minutes from an interview time
 */
function sub_times($time, $int_length)
{
        $splittime = explode(":", $time);

        $min = intval($splittime[1]);
        $hour = intval($splittime[0]);
        $int_length = intval($int_length);

        $min = $min-$int_length;

        while ($min < 0)
        {
                $hour--;
                $min = $min + 60;
        }
        while( $hour < 0 )
        {
                $hour = $hour + 24;
        }
        $time = date( "H:i", mktime( $hour, $min ) );
	$time = $time . ":00";
        return $time;
}
// }}}
// {{{ function sub_times_2($time, $int_length)

/**
 * This function does the same as sub_times, but does not chop off extra time (24 hours are removed in above function)
 */
function sub_times_2($time, $int_length)
{
	$splittime = explode(":", $time);

	$min = intval($splittime[1]);
	$hour = intval($splittime[0]);
	$int_length = intval($int_length);

	$min = $min - $int_length;

	while ($min < 0)
	{
		$hour--;
		$min = $min + 60;
	}
	while ($hour < 0)
	{
		$hour = $hour + 24;
	}

	while ($hour > 23)
	{
		$day++;
		$hour -= 24;
	}

	$time = date("H:i", mktime($hour, $min));
	$split_time = split(":", $time);
	$split_time[0] += $day * 24;
	$time = implode(":", $split_time);

	$time = $time . ":00";

	return $time;
}
// }}}
// {{{ function cal_length($start, $end)
	
/**
 * Calculate the difference (in minutes) between two given times
 */
function cal_length($start, $end)
{
        $start_array = explode(":", $start);
        $end_array = explode(":", $end);

        $start_hour = intval( $start_array[0] );
        $start_min = intval( $start_array[1] );

        $end_hour = intval( $end_array[0] );
        $end_min = intval( $end_array[1] );

        if( $end_hour < $start_hour ) $end_hour = $end_hour + 24;  // should never happen

        return ( ($end_hour - $start_hour) * 60 + $end_min - $start_min );
}
// }}}
// {{{ function getCurrentYear()

/**
 * Get the current year
 */
function getCurrentYear()
{
	$sql = ("
		SELECT DISTINCT CURRENT_DATE
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$current_date = explode('-', $row["CURRENT_DATE"]);
	$current_year = $current_date[0];

	return $current_year;
}
// }}}
// {{{ function getCurrentTime()

/**
 * Function below gets the current time.
 */
function getCurrentTime()
{
	$sql = ("
		SELECT CURRENT_TIME
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$current_time = $row[0];
	
	return($current_time);
}
// }}}
// {{{ function getmicrotime()

/**
 * A function that returns microseconds. Very useful for benchmarking algorithms.
 * @author Code originated from php.net's forums. 
 */
function getmicrotime()
{
    list($usec, $sec) = explode(" ",microtime());
    return ((float)$usec + (float)$sec);
}
// }}}
// {{{ function compareTimes($first, $second)

/**
 * Compare two times and determine which one is greater than the other.
 * Returns 0 if times are equal, 1 if the first time is greater, and 2
 * if the second time is greater.  It is assumed that the two times
 * are on the same day.
 */
function compareTimes($first, $second)
{
	$first_array = explode(":", $first);
	$second_array = explode(":", $second);

	$first_hour = intval($first_array[0]);
	$second_hour = intval($second_array[0]);

	$first_min = intval($first_array[1]);
	$second_min = intval($second_array[1]);

	if ($first_hour > $second_hour)
	{
		return 1;
	}
	elseif ($first_hour < $second_hour)
	{
		return 2;
	}
	elseif ($first_hour == $second_hour)
	{
		if ($first_min > $second_min)
		{
			return 1;
		}
		elseif ($first_min < $second_min)
		{
			return 2;
		}
		else
		{
			return 0;
		}
	}
}
// }}}
// {{{ function count_days($start, $end)
/**
 * Count_days takes two mysql style dates and calculates the days between them. (Handles dates like 2004-04-00)
 * @param $start YYYY-MM-DD
 * @param $end YYYY-MM-DD
 * @return int Number of days between time stamps. 
 * @author Function care of Balin from PHP.net
 */
function count_days($start, $end)
{
    if( $start != '0000-00-00' and $end != '0000-00-00' )
    {         
        $timestamp_start = strtotime($start);
        $timestamp_end = strtotime($end);
        if( $timestamp_start >= $timestamp_end ) return 0;
        $start_year = date("Y",$timestamp_start);
        $end_year = date("Y", $timestamp_end);       
        $num_days_start = date("z",strtotime($start));      
        $num_days_end = date("z", strtotime($end));                
        $num_days = 0;                                                    
        $i = 0;                                                                  
        if( $end_year > $start_year )                                                   
        {                                                                                          
            while( $i < ( $end_year - $start_year ) )                                                             
            {                                                                                                                    
                $num_days = $num_days + date("z", strtotime(($start_year + $i)."-12-31"));                                                        
                $i++;                                                                                                                                          
            }
        }                                                                                                                                            
        return ( $num_days_end + $num_days ) - $num_days_start;                                                                                      
    }                                                                                                                                                
    else                                                                                                                                             
    {                                                                                                                                                
        return 0;                                                                                                                                    
    }                                                                                                                                                
}

// }}}
// {{{ function getMonth($month)

function getMonth ($month)
{
    $month_name = "";
    switch($month){
    case 1:
        $month_name = "January";
        break;
    case 2:
        $month_name = "Feburary";
        break;
    case 3:
        $month_name = "March";
        break;
    case 4:
        $month_name = "April";
        break;
    case 5:
        $month_name = "May";
        break;
    case 6:
        $month_name = "June";
        break;
    case 7:
        $month_name = "July";
        break;
    case 8:
        $month_name = "August";
        break;
    case 9:
        $month_name = "September";
        break;
    case 10:
        $month_name = "October";
        break;
    case 11:
        $month_name = "November";
        break;
    case 12:
        $month_name = "December";
        break;
    }
    return($month_name);
}
    // }}}
    
// }}}
// {{{ Array Functions
// {{{ function trim_elements($array)
/**
 * purpose: removes white space at the beginning and end of each string in an array
 * @param array $array The array to trim.
 * @return array The trimmed array.
 */
function trim_elements($array)
{
   if(is_array($array) && sizeof($array) > 0)
   {
        foreach($array as $index => $value)
        {
            $array[$index] = trim($value);
        }
   }
   return $array;
}

// }}}
// {{{ function unique_elements($array)
/**
 * purpose: returns true if each element in an array is unique (amongst themselves)
 * @param array $array The array to test.
 * @return boolean True if the array is has unique elements.
 */
function unique_elements($array)
{
   $length = count($array);
   for($i = 0; $i < $length; $i++)
   {
      for($j = $i + 1; $j < $length; $j++)
      {
         if ($array[$i] == $array[$j]) { return 0; }
      }
   }
   return 1;
} 

// }}}
// {{{ function unique_arrays($array1, $array2)
/**
 * purpose: returns true if no element in one array appears in the other 
 * @param array $array1 Array number one to test.
 * @param array $array2 Array number two to test.
 * @return boolean True if the arrays are unique.
 */
function unique_arrays($array1, $array2)
{
   $length = count($array1);
   for($i = 0; $i < $length; $i++)
   {
      if (in_array($array1[$i], $array2)) { return 0; }
   }
   return 1;
} 

// }}}
// {{{ function arrayToJS($theArray, $jsArrayName)

/**
 * Name:    arrayToJS
 * Purpose: Returns a JavaScript array from a PHP array (any combination of associative / numeric sub-arrays)
 *
 * Usage:   Echo a php call to this function within Javascript tags. For example:
 *        echo("<script language='JavaScript' type='text/javascript'>");
 *        echo("var myJSArray;");
 *        echo(arrayToJS ($myArray, "myJSArray"));
 *        echo("</script>");
 *
 * @param array $theArray The PHP array to be converted.
 * @param string $jsArrayName The name of the JS array to be created.
 * @return string A line of text which is JS code to initialize the converted the array.         
 */
function arrayToJS($theArray, $jsArrayName)
{
    $jsArrayString = $jsArrayName . " = new Array ();\n";  //the text line of JS code which initializes the array
    foreach ($theArray as $key => $val)                    //for every element at this level of the array
    {
        if (is_numeric ($key))                             //if the key (index) numeric?
        {
            $jsElement = $key;                             //then store it directly
        }
        else                                               //if not?
        {
            $jsElement = "\'" . $key . "\'";               //store with quotes
        }

        if (is_array ($val))                               //if we have a subarray
        {   
            //then recursively call this function with the subarray
            $jsArrayString .= arrayToJS ($val, $jsArrayName . "[" . $jsElement . "]");  
        }
        else                                               //we just have an element
        {
            if (is_numeric ($val))                         //do we have a numeric element?
            {
                $jsValue = $val;                           //then store directly 
            }
            else                                           //if not?
            {
                $jsValue = "\'" . $val . "\'";             //then store with quotes
            }
            
            //create JS code to store the values in new JS array
            $jsArrayString .= $jsArrayName . "[" . $jsElement . "] = " . $jsValue . ";\n";
        }
    }

    return $jsArrayString;
}
// }}}
// {{{ function remove_element(&$src_array, $index_or_value)

/**
 * Remove string element out from array
 */
function remove_element(&$src_array, $index_or_value)
{
        assert( isset($src_array) && is_array($src_array) );

        if( is_int($index_or_value) )
        {
                // remove the element by index
                if( $index_or_value < 0 || $index_or_value >= count($src_array) ) { return false; }

                $index = $index_or_value;

                for( $i = 0; $i < $index ; $i++)
                {
                        array_push( $src_array, array_shift($src_array) );
                }

                array_shift($src_array); // remove the element

                for( $i = 0; $i < $index; $i++ )
                {
                        array_unshift( $src_array, array_pop($src_array) ); // restore those elements originally indexed before the removed element
                }
                return true;
        }
        else
        {
                //remove the element by specified value
                assert( is_string($index_or_value) );

                $value = $index_or_value;

                for( $i = 0; $i < count( $src_array ); $i++ )
                {
                        if( $src_array[$i] == $value )
                        {
                                $index = $i;
                                break;
                        }
                }
                if( $index )
                {
                        for( $i = 0; $i < $index ; $i++)
                        {
                                array_push( $src_array, array_shift($src_array) );
                        }

                        array_shift($src_array); // remove the element

                        for( $i = 0; $i < $index; $i++ )
                        {
                                array_unshift( $src_array, array_pop($src_array) ); // restore those elements originally indexed before the removed element
                        }
                        return true;
                }
                else
                {
			return false; //not found
                }
        }
}
// }}}
// {{{ function isDuplicate($param, &$duplicate)

/**
 * Description: check if duplicated element exists
 */
function isDuplicate($param, &$duplicate)
{

        if ( count($param) <= 1 ) return false;

        sort($param);

        for($i = 0; $i < (count($param) - 1); $i++)
        {
                if( $param[$i] == $param[$i+1] )
                {
                        $duplicate = $param[$i];
                        return true;
                }
        }
        return false;                                                           //no duplicate values
}
// }}}
// {{{ function trim_array($array)

/*
 Function below trims an array of all it's empty elements.  This function is used after receiving an array created with
 checkboxes from a form.
*/

function trim_array($array)
{
    if(!$array)
    {
        return false;
    }
    if(!is_array($array))
    {
        return false;
    }

    for ($i = 0; $i < sizeof($array); $i++)
    {
        if ($array[$i] != '')
        {
            $temp_arr[] = $array[$i];
        }
    }
    $array = $temp_arr;
    return $array;
}

// }}}
// {{{ function break_names($names)
/**
 * @return array Array ([][]["first_name"], [][]["last_name"]) - a list of all the possible first_name last name combinations
 * ex.
 * break_names(Array("Fred(Wen Chan CHANG", " Ross Borden", "Stephen McGrath", "Stephen"))
 * would return the following array
 * Array(
 *  	Array(
 *  		Array("first_name"=>"Fred(Wen",last_name=>"Chan CHANG"),
 *  		Array("first_name"=>"Fred(Wen Chan",last_name=>"CHANG")
 *  	),
 *  	Array(
 *  		Array("first_name"=>"",last_name=>"Ross Borden"),
 *  		Array("first_name"=>" Ross",last_name=>"Borden")
 *  	),
 *  	Array(
 *  		Array("first_name"=>"Stephen",last_name=>"McGrath")
 *  	),
 *  	Array(
 *  		Array("first_name"=>"",last_name=>"Stephen"),
 *  		Array("first_name"=>"Stephen",last_name=>"")
 *  	),
 * )
 */
function break_names($names)
{
	$names_length = sizeof($names);
	for ($i = 0; $i < $names_length; $i++)
	{
		$name_number = 0;
		$name = explode(" ", $names[$i]);
	
		$length = count($name);
		if ($length == 1)
		{
			$names_return[$i][$name_number]["first_name"] = "";
			$names_return[$i][$name_number]["last_name"] = $name[0];
			$name_number++;
			$names_return[$i][$name_number]["first_name"] = $name[0];
			$names_return[$i][$name_number]["last_name"] = "";
			$name_number++;
		}
		else
		{
			for ($j = 1; $j < $length; $j++)
			{
				$names_return[$i][$name_number]["first_name"] = implode(" ", array_slice($name, 0, $j));
				$names_return[$i][$name_number]["last_name"] = implode(" ", array_slice($name, $j, $length));
				$name_number++;
			}
		}	
	}
	return $names_return;
}

// }}}
// {{{ function removeSlashes($param)
/**
 * Remove slashes in the given string or array of strings
 * @param mixed $param Either a string, or an array of strings.
 * @return mixed Either the string with the slashes removed, or an array full of them
 *               depending on the input.
 */
function removeSlashes($param)
{
 	if( is_string( $param ) )
 	{
 		return stripslashes( $param );
 	}
 	elseif( is_array( $param ) && sizeof($param) > 0)
 	{
        foreach($param as $i => $value)
        {
            if (is_string($param[$i]))
            {
                $param[$i] = stripslashes($param[$i]);
            }
        }
		return $param;	
 	}
} // }}}
// {{{ function array_diff_both($new,$old)
function array_diff_both($new,$old){
    $del=array_diff($old,$new);
    $add=array_diff($new,$old);
    return $diff=array("del"=>$del, "add"=>$add);
} // }}}
// {{{ function array_diff_keys_both($new, $old)
function array_diff_keys_both($new,$old){
    $del=array_diff_assoc($old,$new);
    $add=array_diff_assoc($new,$old);
    return $diff=array("del"=>$del, "add"=>$add);
}
// }}}
// }}}
// {{{ Other (unsorted) Functions Here

// {{{ function packObject($object)
/**
 * This function packs an object that needs to be placed in a form value, or 
 * any other place where it is at potential risk of being modified.
 * The function detects whether the mcrypt library is installed to determine 
 * the encoding to use.
 *
 * If the mcrypt library is not found, the object is seralized, deflated, and base64
 * encoded (in that order).
 *
 * If the mcrypt library is found, the object is serialized, encrypted with PACK_SECRET_KEY,
 * deflated, and base64 encoded (in that order).
 * @param object object The object to be packed.
 * @return string The packed object data.
 * @author Patrick Wilkerson
 * @see unpackObject()
 */


function packObject($object){
    if (!defined("PACK_COMPRESSION_LEVEL")) { DEFINE("PACK_COMPRESSION_LEVEL", 1); }
    if (!extension_loaded('mcrypt') || !defined("PACK_SECRET_KEY") || !defined("PACK_CRYPT_TYPE") || !defined("PACK_CRYPT_MODE") || PACK_SECRET_KEY == "" ){ //Check for library and defines.
        $encoded_object = base64_encode(gzdeflate(serialize($object), PACK_COMPRESSION_LEVEL)); //No library, just basic encoding.
    } else {
        $iv_size = mcrypt_get_iv_size(PACK_CRYPT_TYPE, PACK_CRYPT_MODE); //Find the blocksize for the IV (Initialization Vector)
        $iv = mcrypt_create_iv($iv_size, crc32(md5(PACK_SECRET_KEY))); //Get the IV, based off the crc of the md5 of the secret key.  The crc makes it an int, the md5 adds randomness.
        $encrypted_object = mcrypt_encrypt(PACK_CRYPT_TYPE, PACK_SECRET_KEY, gzdeflate(serialize($object), PACK_COMPRESSION_LEVEL), PACK_CRYPT_MODE, $iv); // Encrypt the serialized object. 
        $encoded_object = base64_encode(gzdeflate($encrypted_object, PACK_COMPRESSION_LEVEL)); //Encode so it doesnt break XHTML
    }
    return "OBJ!".$encoded_object; //Add an identifier, so we know its a packed object.
}
// }}}
// {{{ function unpackObject($object)
/**
 * This function unpacks an object that was packed with packObject().
 * The unpack order is opposite that in packObject().
 * @param string $encoded_object The packed data.
 * @param boolean $allow_old_decode Whether to allow old style unpacking (security risk)
 * @return object The original object.
 * @see packObject()
 */
function unpackObject($encoded_object, $allow_old_decode = true){
    //Detect if this is actually a packed object, as if it isnt, many many errors. Also, no object.
    if (substr($encoded_object, 0, 4) != "OBJ!"){ //Fallback to alternate unpacking
        if ($allow_old_decode){
            $object = recurseDetectAndUnpack($encoded_object);
        }
        return $object; 
    } else { //Good string.  Strip the identifier.
        $encoded_object = substr($encoded_object, 4);
    }
    if(!extension_loaded('mcrypt') || !defined("PACK_SECRET_KEY") || !defined("PACK_CRYPT_TYPE") || !defined("PACK_CRYPT_MODE") || PACK_SECRET_KEY == ""){ //Check for library and defines.
        $object = unserialize(gzinflate(base64_decode($encoded_object))); //No library, just basic decoding.
    } else {
        $iv_size = mcrypt_get_iv_size(PACK_CRYPT_TYPE, PACK_CRYPT_MODE);
        $iv = mcrypt_create_iv($iv_size, crc32(md5(PACK_SECRET_KEY)));
        $encrypted_object = gzinflate(base64_decode($encoded_object));
        $object = unserialize(gzinflate(mcrypt_decrypt(PACK_CRYPT_TYPE, PACK_SECRET_KEY, $encrypted_object, PACK_CRYPT_MODE, $iv)));
    }
    return $object;
}
// }}}
// {{{ recurseDetectAndUnpack($object, $tabs)
function recurseDetectAndUnpack($object, $tabs = ""){
    //print $tabs ."ENTERING WITH $object\n";
    if(is_string($object)){
        //print $tabs .gettype($object)." IS STRING\n";
        $object = urldecode(($object));// Dont need any detection here, 
        if(unserialize($object) !== false || $object == "b:0;"){
           $object = unserialize($object);
           //print $tabs ." UNSERIALIZING\n";
        }
        //print $tabs ."NOW ITS A ".gettype($object)."\n";
   } else {
       //print $tabs .gettype($object)." IS NOT STRING";
   }
   //print $tabs ." continuing with object of type ".gettype($object)."\n";
    switch (gettype($object)){
        case "array":
            foreach($object as $key => $val){
               // print $tabs ."SENDING ARRAY STRING $val\n";
              $object[$key] = recurseDetectAndUnpack($object[$key], $tabs . "  ");
            }
            break;
        case "object":
            $class_vars = get_object_vars($object);
            //var_dump($class_vars);
            //var_dump($object);
            foreach($class_vars as $key => $val){

                //print $tabs . "SENDING OBJECT STRING " . $temp ."\n";
                //var_dump( $object->$key);
                $evalstring = '$object->' .$key.' = recurseDetectAndUnpack($object->'.$key.', $tabs . "  ");';
                eval($evalstring);
                //print $tabs ."NEW THING IS ". $object->$key;
            }
            break;
    }
   //print $tabs . "LEAVING<br/>\n";
    return $object;
}

// }}}
// {{{ function getStudentEmailDepartment($student)
/**
 * Returns the student department id (only used for the purpose of e-mails to the employer.)
 * @param $student int The student ID number
 * @return int The department ID number.
 */
function getStudentEmailDepartment($student){
	global $auth;		
	$arr=department_groups($auth->department);
	$department_in_str=implode(",",$arr);

	$sql="Select department_id from student_department where department_id IN ('".$department_in_str."') and student_number='".$student."'";
	$results=$GLOBALS['dbh']->Execute($sql);
	if ($results->RecordCount()>1){
		while ($row=$results->FetchRow()){
			if ($auth->department==$row['department_id']){
				$department_id=$row['department_id'];
			}else{
				$department_id_holder=$row['department_id'];
			}
		}
		if ($department_id==NULL){
			$department_id=$department_id_holder;
		}
	}elseif($results->RecordCount()==1){
		$row=$results->FetchRow();
		$department_id=$row['department_id'];
	}else{
		$sql="Select department_id from student_department where student_number='".$student."' order by record_id desc";
		$results=$GLOBALS['dbh']->Execute($sql);
		$row=$results->FetchRow();
		$department_id=$row['department_id'];
	}
	return $department_id;
	
}
// }}}
// {{{ function success_check($result, $table)
/**
 * purpose: checks if a table was updated correctly
 * @param $result The result to test.
 * @param string $table The table name to report if an error occurs.
 */
function success_check($result, $table)
{
   global $error;

   if ($result == 0)
   {
      $error = 1;
      echo("An error occured while updating table " . $table . "<br />");
   }
}

// }}}
// {{{ function check_name_is($job_id, $student_num)
/**
 * purpose: checks if the 'student number' is shortlisted for 'job id'.
 * @param int $job_id The job ID to check.
 * @param int $student_num The student number to see if shortlisted.
 * @return boolean True if shortlisted.
 */
function check_name_is($job_id, $student_num)
{
   $result = $GLOBALS['dbh']->Execute("
   	SELECT	student_number, job_id
   	FROM		students_shortlisted
   	WHERE		job_id='" . addslashes($job_id) . "' AND
   				student_number='" . addslashes($student_num) . "'");
	if (!$result) { return false; }
	$check_name_is = (($row = $result->FetchRow()) && ($row["job_id"] == $job_id)) ? $row["student_number"] : false;
	$result->Close();

   return $check_name_is;
}

// }}}
// {{{ function check_name_i($job_id, $student_num)
/**
 * purpose: checks if the 'student number' has signed up for 'job_id'
 * @param int $job_id The job ID.
 * @param int $student_number The student number to check for.
 * @return mixed False if not found, otherwise the student number is returned.
 */
function check_name_i($job_id, $student_num)
{
   $result = $GLOBALS['dbh']->Execute("
   	SELECT	student_number, job_id
   	FROM		interview_time
   	WHERE 	job_id='" . addslashes($job_id) . "' AND
   				student_number='" . addslashes($student_num) . "'");
	if (!$result) { return false; }
	$check_name_i = (($row = $result->FetchRow()) && ($row["job_id"] == $job_id)) ? $row["student_number"] : false;
	$result->Close();

   return $row["student_number"];
}

// }}}
// {{{ function check_name_admin($job_id, $time_id)
/**
 * purpose: returns the student_number (or lack thereof) for a time_slot
 * @param int $job_id The job id.
 * @param int $time_id The time id.
 * @return boolean False if none, the student number otherwise.
 */
function check_name_admin($job_id, $time_id)
{
   // check to see if a time slot is empty or not, if empty, returns ''
   $result = $GLOBALS['dbh']->Execute("
   	SELECT	student_number, job_id
   	FROM		interview_time
   	WHERE		job_id='" . addslashes($job_id) . "' AND
   				time_id='" . addslashes($time_id) . "'");
	if (!$result) { return false; }
	$check_name_admin = (($row = $result->FetchRow()) && ($row["job_id"] == $job_id)) ? $row["student_number"] : false;
	$result->Close();
	
   return $check_name_admin;
}

// }}}
// {{{ function isNumber($value)
/**
 * purpose: returns true if a number, false otherwise
 * @param mixed $value The value to test, be it a float/int/string.
 * @return mixed True if is numeric.
 * @see preg_match()
 */
function isNumber($value)
{
	/* return preg_match("/^([1-9][0-9]*)*[0-9](\.[0-9]*)?$/", $value);
     * :EXPIRY: Patrick  - July 5 2005
     */
    return is_numeric($value);
}


// }}}
// {{{ function isInteger($value)
/**
 * purpose: returns true if an integer or integer string, false otherwise
 * @param $value The value to test.
 * @return mixed True if is numeric.
 * @see preg_match()
 */
function isInteger($value)
{
	/* return preg_match("/^([1-9][0-9]*)*[0-9]$/", $value);
     * :EXPIRY: Patrick  - July 5 2005
     */
    return is_numeric($value) && round($value) == $value;
}

// }}}
// {{{ function isJobHostedByDept($job_id, $department_str)
/**
 * Function checks to make sure that the job passed in is hosted by the
 * departments that were passed in.
 */

function isJobHostedByDept($job_id, $department_str)
{
	if ($department_str == '')
	{
		return false;
	}

	$sql = ("
		SELECT DISTINCT job_id
		FROM job_info
		WHERE job_id='" . $job_id . "'
		AND department_id IN (" . $department_str . ")
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	$isJobHostedByDept = $result->RecordCount();

	return $isJobHostedByDept;
}

// }}}
// {{{ function isStudentInDept($student_num, $department_str)
/**
 * Function below checks to make sure that the student number is that of a
 * student in one of the departments passed in.
 */

function isStudentInDept($student_num, $department_str)
{
	if ($department_str == '')
	{
		return false;
	}

	$sql = ("
		SELECT DISTINCT s.student_number
		FROM student AS s, student_department AS sd
		WHERE s.student_number='" . $student_num . "'
		AND sd.department_id IN (" . $department_str . ")
		AND sd.student_number=s.student_number
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	$isStudentInDept = $result->RecordCount();
	
	return $isStudentInDept;
}

// }}}
// {{{ function isSignUpComplete(int $job_id)
/**
 * @return boolean true  - if all students shortlisted for $job_id have signed up
 *                 false - if any students shortlisted for $job_id have not signed up
 */
function isSignUpComplete($job_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	b.time_id, a.student_number
		FROM		students_shortlisted AS a
		LEFT OUTER JOIN interview_time AS b
		ON			a.student_number=b.student_number
		AND		b.job_id='" . addslashes($job_id) . "'
		WHERE		a.job_id='" . addslashes($job_id) . "' AND
					ISNULL(b.time_id)");

	$isSignUpComplete = ($result && !$result->RecordCount());
	$result->Close();

	return $isSignUpComplete;
}

// }}}
// {{{ function isStudentSignedUp($job_id, $student_number)
/**
 * returns: true  - if $student_number is signed_up up for $job_id
 *         false - if $student_number is not signed_up up for $job_id
 */
function isStudentSignedUp($job_id, $student_number)
{
	$sql = ("
        SELECT DISTINCT interview_job_id
        FROM interview_job_join
        WHERE job_id='" . $job_id . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    if ($result->RecordCount())
    {
        // This is a job that is part of a multi-job interview.
        $row = $result->FetchRow();
        $job_id = $row["interview_job_id"];
    }

    $sql = ("
		SELECT DISTINCT job_id AS signed_up
		FROM interview_time
		WHERE job_id='" . addslashes($job_id) . "' 
		AND student_number='" . addslashes($student_number) . "'
        ");

	$result = $GLOBALS['dbh']->Execute($sql);

	if (!$result) { return false; }
	$isStudentSignedUp = ($result->RecordCount()) ? true : false;
	$result->Close();
	
	return $isStudentSignedUp;
}

// }}}
// {{{ function isJobCancelled($job_id)
/**
 * @return boolean true  - if $job_id is cancelled
 *                 false - if $job_id is not cancelled
 */
function isJobCancelled($job_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT job_id
		FROM 	interview
		WHERE job_id='" . addslashes($job_id) . "' AND
				cancelled=1");
	
	if (!$result) { return false; }
	$isJobCancelled = ($result->RecordCount()) ? true : false;
	$result->Close();
	
	return $isJobCancelled;
}

// }}}
// {{{ function isShowable($job_id)
/**
 * @return boolean true  - if $job_id has showable flag set
 *                 false - if $job_id does not have showable flag set
 */
function isShowable($job_id)
{
	$sql = ("
		SELECT DISTINCT interview_job_id
		FROM interview_job_join
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if ($result->RecordCount())
	{
		// This is a job that is part of a multi-job interview.
		$row = $result->FetchRow();
		$job_id = $row["interview_job_id"];
	}

	$sql = ("
		SELECT  job_id	
		FROM	interview
		WHERE	job_id='" . addslashes($job_id) . "' AND student_visible = 1");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if (!$result) { return false; }
	$isShowable = ($result->RecordCount() == 1) && ($row = $result->FetchRow()) && ($row["job_id"] == $job_id);
	$result->Close();
	return $isShowable;
}

// }}}
// {{{ function isJobIDStudentEditable($job_id, $student_number, $job_date)
/**
 * @return boolean true  - if $job_id should be editable by $student_number
 *                 false - if $job_id should not be editable by $student_number, ie, interview date is today.
 */
function isJobIDStudentEditable($job_id, $student_number, $job_date)
{
	// Is job student editable
	// 1. job must not be cancelled
	// 2. job must be showable
	// 3. employer e-mail must not be sent unless interviews span
	//    multiple days. *Fix added by Adam, 2001-01-25.*
	// 4. student_number must be shortlisted
	// 5. the first interview date must be in the future.  *UPDATE*
	//    The interview date they are trying to sign up for must be
	//    in the future.  *Fix added by Adam, 2001-01-25.*
	
	if ((isJobCancelled($job_id)) || (!isShowable($job_id)) || (isEmployerEmailSent($job_id) && !isJobMultipleDays($job_id)) || (!isStudentShortlisted($job_id, $student_number))) 
	{
		return false;
	}

	$result = $GLOBALS['dbh']->Execute("
		SELECT	job_id, IF(int_date<=CURRENT_DATE, 0, 1) AS in_future
		FROM		interview_time
		WHERE		job_id='" . addslashes($job_id) . "' AND int_date ='" . addslashes($job_date) . "'
		ORDER BY	int_date
		LIMIT		0,1");
	
	if (!$result) { return false; }
	$isJobIDStudentEditable = (($row = $result->FetchRow()) && ($row["job_id"] == $job_id) && ($row["in_future"]));
	$result->Close();
	
	return $isJobIDStudentEditable;
}

// }}}
// {{{ function getUnsignedStudents(int $job_id)
/**
 * @return mixed Array of unsigned student numbers for $job_id
 *	             false - on failed query
 */
function getUnsignedStudents($job_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	a.student_number, CONCAT(c.first_name, ' ', c.last_name) AS student_name
		FROM		students_shortlisted AS a, student AS c
		LEFT OUTER JOIN	interview_time AS b
		ON			a.student_number=b.student_number AND
					b.job_id='" . addslashes($job_id) . "'
		WHERE		a.job_id='" . addslashes($job_id) . "' AND
					(ISNULL(b.student_number) OR b.student_number='') AND
					a.student_number=c.student_number
		ORDER BY	last_name, first_name");

	if (!$result) { return false; }
	
	$getUnsignedStudents = Array();
	$i = 0;
	while ($row = $result->FetchRow())
	{
		$getUnsignedStudents[$i]["student_number"] = $row["student_number"];
		$getUnsignedStudents[$i]["student_name"] = $row["student_name"];
		$i++;
	}
	$result->Close();

	return $getUnsignedStudents;
}

// }}}
// {{{ function getStudentName(str $student_number)
/**
 * @return mixed string -  student name for student number
 *	             false - on failed query or no  matching student number
 */
function getStudentName($student_number)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	CONCAT(first_name, ' ', last_name) AS student_name
		FROM		student
		WHERE		student_number='" . addslashes($student_number) . "'");

	if (!$result) { return false; }
	$getStudentName = (($result->RecordCount() == 1) && ($row = $result->FetchRow())) ? $row["student_name"] : false;
	$result->Close();
	
	return $getStudentName;
}

// }}}
// {{{ function getCiEmail(str $login_id)
/**
 * Get contact internal e-mail
 * @return mixed str   - e-mail of contact internal with login_id
 *          false - query failed, no e-mail, or no matching contact
 */
function getCiEmail($login_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	b.email
		FROM		contact_internal AS a,
					contact AS b
		WHERE		a.login_id='" . addslashes($login_id) . "' AND
					a.contact_id=b.contact_id");
	if (!$result) { return false; }
	$getCiEmail = (($result->RecordCount() == 1) && ($row = $result->FetchRow())) ? $row["email"] : false;
	$result->Close();
	
	return $getCiEmail;
}

// }}}
// {{{ function getCiEmailUsingID(str $contact_id)
/**
 * @return mixed str - e-mail of contact internal with contact_id
 * 	    false - query failed, no e-mail, or no matching contact.
 */
function getCiEmailUsingID($contact_id)
{
	$sql = ("
		SELECT DISTINCT email
		FROM contact
		WHERE contact_id='" . $contact_id . "'
		AND email IS NOT NULL AND email != ''
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount() > 1)
	{
		$getCiEmailUsingID = false;
	}
	elseif ($result->RecordCount())
	{
		$row = $result->FetchRow();
		$getCiEmailUsingID = $row["email"];
	}
	else
	{
		/*
		 They didn't have an e-mail listed for this contact id, so generate one
		 using their login/netlink id and their departments e-mail suffix.
		*/

		$sql = ("
			SELECT DISTINCT a.email, b.department_id, b.login_id, b.netlink_id, a.default_login
			FROM department AS a, contact_internal AS b
			WHERE b.contact_id='" . $contact_id . "'
			AND a.department_id=b.department_id
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		
		if ($result->RecordCount() > 1)
		{
			$getCiEmailUsingID = false;
		}
		elseif ($result->RecordCount())
		{
			$row = $result->FetchRow();

            // We have two ways of deriving e-mail addresses based on a user's login. If the user's department uses NIS for login, then we know that the 
            // user's email address is NIS_LOGIN@department.domain.whatever. So, we can append the department table's e-mail suffix with the user's NIS login id.
            if ($row['default_login'] == NIS_LOGIN)
			{
				if ($row["login_id"] == '*')
				{
					$getCiEmailUsingID = false;
				}
				else
				{
					$getCiEmailUsingID = ($row["login_id"] . "@" . $row["email"]);
				}
			}
            // Otherwise, user is using netlink which is another way of logging in. Their password is always netlink_id@netlink.domain.whatever. 
			else
			{
				if ($row["netlink_id"] == '*')
				{
					$getCiEmailUsingID = false;
				}
				else
				{
					$getCiEmailUsingID = ($row["netlink_id"] . "@" . $row["email"]);
				}
			}
		}
		else
		{
			$getCiEmailUsingID = false;
		}
	}
		
	return $getCiEmailUsingID;
}

// }}}
// {{{ function getContactID($department_id, $login_id, $NIS_Netlink = '')
/*
 * Function: getContactID
 * Description: Function takes a department ID and a login ID as parameters and returns the
 * associated contact ID.  This function will take into account login_id's and netlink_id's.
 */

function getContactID($department_id, $login_id, $NIS_Netlink = '')
{
	if ($NIS_Netlink == NIS_LOGIN)
	{
		$default_login = NIS_LOGIN;
	}
	elseif ($NIS_Netlink)
	{
		$default_login = $NIS_Netlink;
	}
	else
	{
		/*
		 Get the default login for this department first.
		*/
	
		$sql = ("
			SELECT DISTINCT default_login
			FROM department
			WHERE department_id='" . addslashes($department_id) . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$default_login = $row["default_login"];
	
		if (!(trim($department_id)) || !(trim($login_id)))
		{
			$getContactID = false;
		}
	}
	
	if ($default_login == NIS_LOGIN)
	{
		$sql = ("
			SELECT DISTINCT contact_id
			FROM contact_internal
			WHERE department_id='" . addslashes($department_id) . "'
			AND login_id='" . addslashes($login_id) . "'
			");
        $result = $GLOBALS['dbh']->Execute($sql);

        if (!$result->RecordCount()) {
            // Contact isn't using the default login.
            $sql = ("
                SELECT DISTINCT contact_id
                FROM contact_internal
                WHERE department_id='" . addslashes($department_id) . "'
                AND netlink_id='" . addslashes($login_id) . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }
	}
	else
	{
		$sql = ("
			SELECT DISTINCT contact_id
			FROM contact_internal
			WHERE department_id='" . addslashes($department_id) . "'
			AND netlink_id='" . addslashes($login_id) . "'
			");

        $result = $GLOBALS['dbh']->Execute($sql);

        if (!$result->RecordCount()) {
            // Contact isn't using the default login.
            $sql = ("
                SELECT DISTINCT contact_id
                FROM contact_internal
                WHERE department_id='" . addslashes($department_id) . "'
                AND login_id='" . addslashes($login_id) . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }
	}

	if ($result->RecordCount() > 1)
	{
		$getContactID = false;
	}
	elseif ($result->RecordCount())
	{
		$row = $result->FetchRow();
		$getContactID = $row["contact_id"];
	}
	else
	{
		$getContactID = false;
	}

	return $getContactID;
}
		
// }}}
// {{{ function getContactInfoByLoginID($login_id)
/**
 * Get contact and contact_internal info
 * @return mixed Array - a hash containing all the fields from contact and contact_internal for $login_id
 *          false - query failed or no matching $login_id
 */
function getContactInfoByLoginID($login_id)
{
    global $auth;
	$result = $GLOBALS['dbh']->Execute("
		SELECT *
		FROM contact AS a
        INNER JOIN contact_internal AS b
            ON a.contact_id=b.contact_id
		WHERE (b.login_id='" . addslashes($login_id) . "' OR b.netlink_id='". addslashes($login_id)."') 
        AND department_id = '".$auth->department."'
        ");

	if (!$result) 
    {
        return false; 
    }
	$getContactInfoByLoginID = ($result->RecordCount() == 1) ? $result->FetchRow() : false;
	$result->Close();
	
	return $getContactInfoByLoginID;
}

// }}}
// {{{ function getContactInfoByID($contact_id)
/**
 * Function: getContactInfoByID(str $contact_id)
 * Description: returns an array containing all the fields from contact and contact_internal for this $contact_id
 * or false if the query failed or there is no matching contact id.
 */
function getContactInfoByID($contact_id)
{
	$sql = ("
		SELECT *
		FROM contact AS a, contact_internal AS b
		WHERE b.contact_id='" . addslashes($contact_id) . "'
		AND a.contact_id=b.contact_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	if (!$result)
	{
		return false;
	}

	$getContactInfoByID = (($result->RecordCount() == 1) ? $result->FetchRow() : false);
	$result->Close();

	return $getContactInfoByID;
}

// }}}
// {{{ function getTerm($month)
/**
 * Finds the given term id for a selected month and returns it.
 * @return int The term ID.
 */
function getTerm($month){
	$sql = "SELECT term_id
            FROM term
		    WHERE '" . $month . "' <= end_date AND '" . $month . "' >= start_date";
	$result = $GLOBALS['dbh']->Execute( $sql );

	if (!result || ( $result->RecordCount() <= 0) ) { return 0; };
	$row = $result->FetchRow();
	return $row["term_id"];
}

// }}}
// {{{ function getTermYearOrder($term_id)
function getTermYearOrder($term_id) {
    $sql = "SELECT year_order
            FROM   term
            WHERE  term_id = '$term_id'";
    $result = $GLOBALS['dbh']->Execute($sql);
    $row    = $result->FetchRow();

    return $row[0];
}

// }}}
// {{{ function isJobInPast($job_id)
/**
 * Finds out if the given job has any interviews that aren't in the past.  If 
 * Yes then returns false, otherwise returns true (indicating job IS in the 
 * past.
 */
function isJobInPast($job_id)
{
	global $current_date;

	if (!isValidJobID($job_id))
	{
		return false;
	}
	$isJobInPast = true;
	$result = $GLOBALS['dbh']->Execute("
		SELECT int_date 
		FROM interview_time
		WHERE job_id = '" . $job_id . "'
		");
	while ($row = $result->FetchRow())
	{
		if ($row["int_date"] >= $current_date)
		{
			$isJobInPast = false;
			break;
		}
	}
	
	$result->Close();
	return $isJobInPast;
}
	
// }}}
// {{{ function getDepartmentCode($department_id)
/** 
 * Get the code for a given department
 */
function getDepartmentCode($department_id)
{
	$sql = ("
		SELECT DISTINCT department_code
		FROM department
		WHERE department_id='" . $department_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();

	return ($row["department_code"]);
}

// }}}
// {{{ function getDepartmentName($department_id)
function getDepartmentName($department_id){
	$sql = ("
		SELECT department_name
		FROM department
		WHERE department_id = '" . addslashes($department_id) . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if (!$result){ return ""; }
	$row = $result->FetchRow();
	return $row["department_name"];
}

// }}}
// {{{ function department_groups($department_id)
/*
 This function returns an array containing all of the departments that are in the passed departments
 group.  If there is no group, then the department passed in is returned (in an array).
*/

function department_groups($department_id){
   global $auth;

	$departments_in_group = Array();
	
	if ($auth->userlevel == TRAFFICDIRECTOR) {
		$sql = 	"SELECT DISTINCT department_id
			 FROM department"; 
			 
		$result = $GLOBALS['dbh']->Execute($sql);
		if ($result->RecordCount() <= 0){
			$departments_in_group[] = $department_id;
			return $departments_in_group;
		}
		while ($row = $result->FetchRow()){
			$departments_in_group[] = $row["department_id"];
		}

		return $departments_in_group;
	}

	$sql = ("
		SELECT group_id 
		FROM department_group
		WHERE department_id = '" . addslashes($department_id) . "'");
	$result = $GLOBALS['dbh']->Execute($sql);
		
	if ($result->RecordCount() <= 0){
		$departments_in_group[] = $department_id;
		return $departments_in_group;
	}

	$in_str = "";
	while ($row = $result->FetchRow()){
		$in_str .= "'" . addslashes($row["group_id"]) . "', ";
	}
	$in_str = substr($in_str, 0, strlen($in_str) - 2);
	
	$sql = 	"SELECT DISTINCT department_id
		 FROM department_group 
		 WHERE group_id IN (" . $in_str . ")";
		 
	$result = $GLOBALS['dbh']->Execute($sql);
	if ($result->RecordCount() <= 0){
		$departments_in_group[] = $department_id;
		return $departments_in_group;
	}
	while ($row = $result->FetchRow()){
		$departments_in_group[] = $row["department_id"];
	}

	return $departments_in_group;
}

// }}}
// {{{ function getDisciplineName($discipline_id)
function getDisciplineName($discipline_id)
{
	$sql = ("
		SELECT discipline_name
		FROM discipline
		WHERE discipline_id = '" . addslashes($discipline_id) . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if (!$result->RecordCount())
    { 
        return ""; 
    }
	$row = $result->FetchRow();
	return $row["discipline_name"];
}

// }}}
// {{{ function getCompanyAndDivisionName($contact_id)
/**
 * If the company name is the same as the division name, return only one, otherwise both
 */
function getCompanyAndDivisionName ($contact_id) {

	$sql = "SELECT ce.employer_id, ce.department_id
		FROM contact_employer AS ce
		WHERE ce.contact_id='$contact_id'";
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	$comp_name = trim (getCompanyName ($row["employer_id"]));
	$div_name = trim (getDivisionName ($row["department_id"]));
	
	// now find out if they're the same, 
	// they're the same
	if (strcasecmp ($comp_name, $div_name) == 0)
		return ($comp_name);
	// they're different
	else
		return ($comp_name .", ". $div_name);

}

// }}}
// {{{ function getCompanyName($employer_id)
function getCompanyName($employer_id){
	$result = $GLOBALS['dbh']->Execute("
			SELECT company_name
			FROM employer_company
			WHERE employer_id = '" . addslashes($employer_id) . "'");

	if (!$result){
		return "";
	}
	$row = $result->FetchRow();
	return $row["company_name"];
}

// }}}
// {{{ function getDivisionName($division_id)
function getDivisionName($division_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT department_name
		FROM employer_department
		WHERE department_id='" . addslashes($division_id) . "'
		");
	if (!$result)
	{
		return "";
	}
	$row = $result->FetchRow();
	return ($row["department_name"]);
}

// }}}
// {{{ function getContactName($contact_id)
function getContactName($contact_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT first_name, last_name
		FROM contact
		WHERE contact_id='" . $contact_id . "'
		");
	if (!$result)
	{
		return "";
	}
	$row = $result->FetchRow();
	$contact_name["first_name"] = $row["first_name"];
	$contact_name["last_name"] = $row["last_name"];
	return ($contact_name);
}

// }}}
// {{{ function getEmployerTypeName($id)
/**
 * Functions below return the respective name's associated with the id passed in.
 * @TODO :TODO remove references to this function.  We should only be using getContactStatusName
 */
function getEmployerTypeName($id)
{
        $sql = ("
                SELECT DISTINCT type_name
                FROM contact_types
                WHERE type_id='" . $id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $type = $row["type_name"];
        return $type;
}

// }}}
// {{{ function getCompanyTypeName($id)
function getCompanyTypeName($id)
{
	$sql = ("
		SELECT DISTINCT type_name
		FROM company_type
		WHERE type_id='" . $id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$type = $row["type_name"];
	return $type;
}

// }}}
// {{{ function getAreaName($id)

function getAreaName($id)
{
        $sql = ("
                SELECT DISTINCT area_name
                FROM area_list
                WHERE area_id='" . $id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $area = $row["area_name"];
        return $area;
}

// }}}
// {{{ function getAreaNameFromCountry($id)
function getAreaNameFromCountry($id)
{
        $sql = ("
                SELECT DISTINCT al.area_name
                FROM area_list AS al
                LEFT JOIN country_list AS cl ON cl.area_id=al.area_id
                WHERE cl.country_id='" . $id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $area = $row["area_name"];
        return $area;
}

// }}}
// {{{ function getCountryName($id)
function getCountryName($id)
{
        $sql = ("
                SELECT DISTINCT country_name
                FROM country_list
                WHERE country_id='" . $id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $country = $row["country_name"];
        return $country;
}

// }}}
// {{{ function getProvstateName($id)
function getProvstateName($id)
{
        $sql = ("
                SELECT DISTINCT provstate_name
                FROM provstate_list
                WHERE provstate_id='" . $id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $provstate = $row["provstate_name"];
        return $provstate;
}

// }}}
// {{{ function getRegionName($id)
function getRegionName($id)
{
        $sql = ("
                SELECT DISTINCT region_name
                FROM region_list
                WHERE region_id='" . $id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $region = $row["region_name"];
        return $region;
}

// }}}
// {{{ function getContactStatusName($id)
/**
 * @TODO :TODO OBSOLETE, delete when statuses are finished, we now use getEmployerStatusName($id);
 * @deprecated delete when statuses are finished, we now use getEmployerStatusName($id);
 */
function getContactStatusName($id)
{
    $sql = ("
        SELECT DISTINCT type_name
        FROM contact_types
        WHERE type_id='" . $id . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $getContactStatusName = $row["type_name"];
    return $getContactStatusName;
}

// }}}
// {{{ function getEmployerStatusName($id)

function getEmployerStatusName($id)
{
	$sql = ("
		SELECT status_name
		FROM employer_info_statuses
		WHERE status_id='" . $id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$getEmployerStatusName = $row["status_name"];
	return $getEmployerStatusName;
}

// }}}
// {{{ function getStatusName($id)
function getStatusName($id)
{
	$sql = ("
		SELECT DISTINCT status_name
		FROM job_status
		WHERE status_id='" . $id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$status = $row["status_name"];
	return $status;
}

// }}}
// {{{ function getAdminStatusName($id)
function getAdminStatusName($id)
{
	$sql = ("
		SELECT DISTINCT status_name
		FROM admin_status
		WHERE status_id='" . $id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$admin_status = $row["status_name"];
	return $admin_status;
}

// }}}
// {{{ function getHistoryStatusName($id)
function getHistoryStatusName($id)
{
	$sql = ("
		SELECT DISTINCT history_status_name
		FROM history_status
		WHERE history_status_id='" . $id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$status = $row["history_status_name"];
	return $status;
}

// }}}
// {{{ function getTermName($id)
function getTermName($id)
{
        $sql = ("
                SELECT DISTINCT term_name
                FROM term
                WHERE term_id='" . $id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $term = $row["term_name"];
        return $term;
}

// }}}
// {{{ function getTermMonth($id)
function getTermMonth($id)
{
    $sql = ("
            SELECT DISTINCT start_date
            FROM term
            WHERE term_id='" . $id . "'
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $month = $row["start_date"];
    return $month;
}
// }}}
// {{{ function getSemesterCode($id)
function getSemesterCode($id)
{
    $sql = ("
        SELECT DISTINCT display
        FROM semesters
        WHERE semesters_id='" . $id . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $semesters_code = $row["display"];
    return $semesters_code;
}

// }}}
// {{{ function getSizeRange($id)
function getSizeRange($id)
{
	$sql = ("
		SELECT DISTINCT size_range
		FROM employer_sizes
		WHERE size_id='" . $id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$size = $row["size_range"];
	return $size;
}

// }}}
// {{{ function getDisciplineCode($id)
function getDisciplineCode($id)
{
	$sql = ("
		SELECT DISTINCT discipline_code
		FROM department_disciplines
		WHERE discipline_id='" . $id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$discipline_code = $row["discipline_code"];
	return $discipline_code;
}

// }}}
// {{{ function getFlagName($id, $flag_type)
function getFlagName($id, $flag_type)
{
    switch($flag_type)
    {
        case "student":
            $sql = ("
                    SELECT description
                    FROM student_flags
                    WHERE student_flags_id='" . $id . "'
                    ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $flag = $row["description"];

        return $flag;
        case "history":
            $sql = ("
                    SELECT description
                    FROM history_flags
                    WHERE history_flags_id='" . $id . "'
                    ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $flag = $row["description"];

        return $flag;

        break;
        case "division":
            $sql = ("
                    SELECT flag_name
                    FROM division_flags
                    WHERE flag_id='" . $id . "'
                    ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $flag = $row["flag_name"];

        return $flag;
        case "department":
            $sql = ("
                    SELECT flag_name
                    FROM division_flags
                    WHERE flag_id='" . $id . "'
                    ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $flag = $row["flag_name"];

        return $flag;
        case "contact":
            $sql = ("
                    SELECT flag_name
                    FROM contact_flags
                    WHERE flag_id='" . $id . "'
                    ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $flag = $row["flag_name"];

        return $flag;

        break;
        case "company":
        default:
            $sql = ("
                    SELECT flag_name
                    FROM company_flags
                    WHERE flag_id='" . $id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $flag = $row["flag_name"];

            return $flag;

            break;
    }
}
// }}}
// {{{ function getFlagSharedDeptCode($flag_type, $department_id, $flag_id, $id_or_code) 
/*
 * function getFlagSharedDeptCode
 * @return mixed Either an array of dep codes or dep ids, depending on $id_or_code param
 * @param $flag_type Flag types (so far: History, Student, Company, Contact and Division
 * @param $department_id
 * @param $flag_id Flag identifier in the flags tables
 * @param $id_or_code Specifies which kind of array you would like this function to return
 *                    ex: "code" returns an array of department codes
 *                        "id" returns an array of department_ids
 */
function getFlagSharedDeptCode($flag_type, $department_id, $flag_id, $id_or_code) 
{
    switch($flag_type)
    {
        case "Student":
            $shared_sql = "SELECT d.department_code, dsf.department_id
            FROM department_student_flags AS dsf
            INNER JOIN department AS d
            ON dsf.department_id=d.department_id
            WHERE student_flags_id='".addslashes($flag_id)."'";
        break;
        case "History":
            $shared_sql = "SELECT d.department_code, dsf.department_id
            FROM department_history_flags AS dsf
            INNER JOIN department AS d
            ON dsf.department_id=d.department_id
            WHERE history_flags_id='".addslashes($flag_id)."'";
        break;
        case "Company":
            $shared_sql = "SELECT d.department_code, dsf.department_id
            FROM department_company_flags AS dsf
            INNER JOIN department AS d
            ON dsf.department_id=d.department_id
            WHERE flag_id='".addslashes($flag_id)."'";
        break;
        case "Contact":
            $shared_sql = "SELECT d.department_code, dsf.department_id
            FROM department_contact_flags AS dsf
            INNER JOIN department AS d
            ON dsf.department_id=d.department_id
            WHERE flag_id='".addslashes($flag_id)."'";
        break;
        case "Division":
            $shared_sql = "SELECT d.department_code, dsf.department_id
            FROM department_division_flags AS dsf
            INNER JOIN department AS d
            ON dsf.department_id=d.department_id
            WHERE flag_id='".addslashes($flag_id)."'";
        break;
        default:
        error("Flag type has not been chosen");
        break;
    }
    if($shared_sql){
        $shared_sql .=  "AND d.department_id !='".addslashes($department_id)."'
            ORDER BY d.department_code
            ";
        $shared_array = array();
        $shared_result = $GLOBALS['dbh']->Execute($shared_sql);
        while($shared_row = $shared_result->FetchRow())
        {
            switch($id_or_code)
            {
                case "dept_code":
                    $shared_array[] = $shared_row["department_code"];
                break;
                case "dept_id":
                    $shared_array[] = $shared_row["department_id"];
                break;
            }
        }
        return $shared_array;
    }
}

// }}}
// {{{ function getSalaryPeriodName($id)
function getSalaryPeriodName($id)
{
    if (isInteger($id) == 0)
    {
        return NULL;
    }

    $sql = ("
        SELECT salary_period_name
        FROM salary_period
        WHERE salary_period_id = '".$id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $period_name = $row['salary_period_name'];

    return $period_name;

}

// }}}
// {{{ function getContactTypeName($id)
/**
 * @TODO :TODO Remove references to this function.  We should only be using getContactStatusName
 * @deprecated We should only be using getContactStatusName
 * @see getContactStatusName()
 */
function getContactTypeName($id)
{
	$sql = ("
		SELECT DISTINCT type_name
		FROM contact_types
		WHERE type_id='" . $id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$type = $row["type_name"];

	return $type;
}

// }}}
// {{{ function getIndustryName($id)
function getIndustryName($id)
{
	$sql = ("
		SELECT DISTINCT industry_name
		FROM industries
		WHERE industry_id='" . $id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$industry = $row["industry_name"];

	return $industry;
}

// }}}
// {{{ function getStudentStatusName($id)
function getStudentStatusName($id)
{
	$sql = ("
		SELECT DISTINCT description
		FROM student_status
		WHERE status_id='" . $id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$student_status = $row["description"];

	return $student_status;
}

// }}}
// {{{ function getTitleName($id)
function getTitleName($id)
{
	$sql = ("
		SELECT DISTINCT title_name
		FROM title
		WHERE title_id='" . $id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$title_name = $row["title_name"];

	return $title_name;
}

// }}}
// {{{ function flagInDept($flag_id, $department_id, $contact_dept_company)
/**
 * Function below checks to see if a contact flag is used by the given department
 */
function flagInDept($flag_id, $department_id, $contact_dept_company)
{
	if ($contact_dept_company == 'contact')
	{
		$sql = ("
			SELECT DISTINCT flag_id
			FROM department_contact_flags
			WHERE flag_id='" . $flag_id . "'
			AND department_id='" . $department_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		if ($result->RecordCount())
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}
	elseif ($contact_dept_company == 'department')
	{
		$sql = ("
			SELECT DISTINCT flag_id
			FROM department_division_flags
			WHERE flag_id='" . $flag_id . "'
			AND department_id='" . $department_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		if ($result->RecordCount())
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}
	elseif ($contact_dept_company == 'company')
	{
		$sql = ("
			SELECT DISTINCT flag_id
			FROM department_company_flags
			WHERE flag_id='" . $flag_id . "'
			AND department_id='" . $department_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		if ($result->RecordCount())
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}
}
// }}}
// {{{ function isJobInfoEditable($job_id)
/**
 * Function below takes a job_id as a parameter, and checks to see if the user can edit the job, based on their userlevel.
 */
function isJobInfoEditable($job_id)
{
	global $userlevel;
	global $auth;

	if ($userlevel == OFFICE || $userlevel == ADMINISTRATOR)
	{
		/*
		 Get the groups associated with this department.
		*/

		$depts_group = department_groups($auth->department);

		foreach ($depts_group AS $dg)
		{
			$dept_string .= ("'" . $dg . "',");
		}
		if ($dept_string)
		{
			$dept_string = substr($dept_string, 0, -1);
		}
		else
		{
			$dept_string = ("''");
		}

		$sql = ("
			SELECT CURRENT_DATE
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$current_date = $row["CURRENT_DATE"];
		
		$current_date = explode("-", $current_date);
		$current_year = $current_date[0];
		$current_term = getCurrentTermID();
		$sql = ("
			SELECT DISTINCT t1.term_id
			FROM term AS t1, term AS t2
			WHERE t2.term_id='" . $current_term . "'
			AND t1.year_order>=t2.year_order
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		while ($row = $result->FetchRow())
		{
			$term_ids .= ("'" . $row["term_id"] . "',");
		}
		if ($term_ids)
		{
			$term_ids = substr($term_ids, 0, -1);
		}
		else
		{
			$term_ids = ("''");
		}
			
		$sql = ("
			SELECT DISTINCT department_id, term_id, year
			FROM job_info
			WHERE job_id='" . $job_id . "'
			AND department_id IN (" . $dept_string . ")
			AND (year>'" . $current_year . "' OR (year='" . $current_year . "' AND term_id IN (" . $term_ids . ")))
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		$isJobInfoEditable = ($result->RecordCount());

		return $isJobInfoEditable;
	}
	elseif ($userlevel == TRAFFICDIRECTOR)
	{
		$sql = ("
                        SELECT CURRENT_DATE
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $current_date = $row["CURRENT_DATE"];

                $current_date = explode("-", $current_date);
                $current_year = $current_date[0];
                $current_term = getCurrentTermID();
                $sql = ("
                        SELECT DISTINCT t1.term_id
                        FROM term AS t1, term AS t2
                        WHERE t2.term_id='" . $current_term . "'
                        AND t1.year_order>=t2.year_order
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                while ($row = $result->FetchRow())
                {
                        $term_ids .= ("'" . $row["term_id"] . "',");
                }
                if ($term_ids)
                {
                        $term_ids = substr($term_ids, 0, -1);
                }
                else
                {
                        $term_ids = ("''");
                }

                $sql = ("
                        SELECT DISTINCT job_id
                        FROM job_info
                        WHERE job_id='" . $job_id . "'
                        AND (year>'" . $current_year . "' OR (year='" . $current_year . "' AND term_id IN (" . $term_ids . ")))
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);

                $isJobInfoEditable = ($result->RecordCount());

                return $isJobInfoEditable;
	}
	elseif ($userlevel == EMPLOYER || $userlevel == HREMPLOYER)
	{
		/*
		 Later on, we'll need to do checks on a company's preferences to see who in the company can edit jobs.
		*/
		
		$sql = ("
			SELECT DISTINCT employer_id
			FROM employer_login
			WHERE login_id='" . $auth->login . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		assert($result->RecordCount() < 2);

		$row = $result->FetchRow();

		$sql = ("
			SELECT DISTINCT employer_id, status
			FROM job_info
			WHERE job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		
		assert($result->RecordCount() < 2);
		
		$row2 = $result->FetchRow();

		if ($row["employer_id"] == $row2["employer_id"] && $row2["status"] != 'POSTED')
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
// }}}
// {{{ function getStudentEmail($student_number)

/**
 * Function below takes a student number as parameter, and returns the students e-mail address, or -1 if the student doesn't exist in the database.
 */

function getStudentEmail($student_number)
{
	if (!($student_number = trim($student_number)))
	{
		return false;
	}

	$sql = ("
		SELECT DISTINCT s.email, s.login_id, s.netlink_id
            , sd.department_id
            , d.email AS domain, d.default_login
		FROM student s
        INNER JOIN student_department as sd
          ON s.student_number = sd.student_number
        INNER JOIN department AS d
          ON sd.department_id = d.department_id
		WHERE s.student_number = '" . $student_number . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	if (!($row = $result->FetchRow()))
    {
		return false;
	}

    // check if the student has an email or an email2, otherwise form their email address from login info
    if (trim($row['email']))
    {
        return $row['email'];
    }
    elseif ($row['netlink_id'] != '*' && $row['default_login'] == SCRIPT_LOGIN)
    {
        return $row['netlink_id'].'@'.$row['domain'];
    }
    elseif ($row['login_id'] != '*' && $row['default_login'] == NIS_LOGIN)
    {
        return $row['login_id'].'@'.$row['domain'];
    }
    elseif (SCRIPT_LOGIN == 'netlink')
    {
        // last resort.  form the email by querying netlink.
        $string_code = "Q:S:".$student_number;
        $validator_info[] = "";
        exec(SCRIPT_PATH."/client.pl $string_code", $validator_info);
        list ($return_code, $return_email) = split(":", $validator_info[1]);

        if ($return_code == '1')
        {
            return $return_email;
        } 
        else
        {
            return false;
        }
    }
}
// }}}
// {{{ function recordContactChange($editedContact, $entity)

/*
 * Function below is used to determine what has been changed in a contact's information, upon
 * completion of editing, and whether or not it is necessary for a reason to be entered.
 * The following fields can be edited without a reason:
 *
 * - Contact Flags
 * - General Comments
 * - Department Specific Comments
 *
 * This function returns false if nothing has been changed that needs to be noted, and an
 * array containing the names of the fields that have been changed otherwise.  Also, a
 * field that did not previously contain a value will not be recorded as being changed
 * if it has a value added upon this edit being saved.
 *
 * $editedContact must be loaded with contact values, rather than department or company
 * values for this function to work.
 */

function recordContactChange($editedContact, $entity)
{
	switch($entity)
	{
	
	case "contact":

		$oldContact = new EditContact('');
		$oldContact->loadContactValues($editedContact->contact_id);
	
		if (recordChange($editedContact->contact_title, $oldContact->contact_title))
		{
			$recordString .= "Title, ";
		}
		
		if (recordChange($editedContact->contact_first_name, $oldContact->contact_first_name))
		{
			$recordString .= "First Name, ";
		}
	
		if (recordChange($editedContact->contact_middle_name, $oldContact->contact_middle_name))
		{
			$recordString .= "Middle Name, ";
		}
	
		if (recordChange($editedContact->contact_last_name, $oldContact->contact_last_name))
		{
			$recordString .= "Last Name, ";
		}
	
		if (recordChange($editedContact->contact_called_name, $oldContact->contact_called_name))
		{
			$recordString .= "Called Name, ";
		}
	
		if (recordChange($editedContact->contact_greeting, $oldContact->contact_greeting))
		{
			$recordString .= "Greeting, ";
		}
	
		if (recordChange($editedContact->contact_email, $oldContact->contact_email))
		{
			$recordString .= "E-mail, ";
		}
	
		if (recordChange($editedContact->contact_phone, $oldContact->contact_phone))
		{
			$recordString .= "Phone, ";
		}
	
		if (recordChange($editedContact->contact_pager, $oldContact->contact_pager))
		{
			$recordString .= "Pager, ";
		}
	
		if (recordChange($editedContact->contact_cellphone, $oldContact->contact_cellphone))
		{
			$recordString .= "Cell Phone, ";
		}
		
		if (recordChange($editedContact->contact_fax, $oldContact->contact_fax))
		{
			$recordString .= "Fax, ";
		}
	
        if (recordChange($editedContact->contact_location_info, $oldContact->contact_location_info))
        {
            $recordString .= "Location Info (Using Company's, Using Own, etc.), ";
        }
        elseif ($editedContact->contact_location_info != USE_COMPANY || $editedContact->contact_location_info != USE_DEPARTMENT)
        {
            if (recordChange($editedContact->contact_street_address_1, $oldContact->contact_street_address_1))
            {
                $recordString .= "Street Address 1, ";
            }
        
            if (recordChange($editedContact->contact_street_address_2, $oldContact->contact_street_address_2))
            {
                $recordString .= "Street Address 2, ";
            }

            if (recordChange($editedContact->contact_street_address_3, $oldContact->contact_street_address_3))
            {
                $recordString .= "Street Address 3, ";
            }
        
            if (recordChange($editedContact->contact_city, $oldContact->contact_city))
            {
                $recordString .= "City, ";
            }
        
            if (recordChange($editedContact->contact_region_id, $oldContact->contact_region_id))
            {
                $recordString .= "Region, ";
            }
        
            if (recordChange($editedContact->contact_provstate_id, $oldContact->contact_provstate_id))
            {
                $recordString .= "Province/State, ";
            }
        
            if (recordChange($editedContact->contact_country_id, $oldContact->contact_country_id))
            {
                $recordString .= "Country, ";
            }
        
            if (recordChange($editedContact->contact_postal_code, $oldContact->contact_postal_code))
            {
                $recordString .= "Postal Code, ";
            }
        }
		
		if (recordChange($editedContact->contact_department_name, $oldContact->contact_department_name))
		{
			$recordString .= "Department Name, ";
		}

		if (trim($recordString))
		{
			$recordString = substr($recordString, 0, (strlen($recordString) - 2));
			return $recordString;
		}
		else
		{
			return false;
		}
	
		break;
	
	case "department":

		$oldContact = new EditContact('');
		$oldContact->loadDepartmentValues($editedContact->department_id);

		if (recordChange($editedContact->department_name, $oldContact->department_name))
		{
			$recordString .= "Name, ";
		}
	
		if (recordChange($editedContact->department_website, $oldContact->department_website))
		{
			$recordString .= "Website, ";
		}

		if (recordChange($editedContact->department_description, $oldContact->department_description))
		{
			$recordString .= "Description, ";
		}

        if (recordChange($editedContact->department_location_info, $oldContact->department_location_info))
        {
            $recordString .= "Location Info (Using Company's, Using Own, etc.), ";
        }
        elseif ($editedContact->department_location_info != USE_COMPANY)
        {
            if (recordChange($editedContact->department_street_address_1, $oldContact->department_street_address_1))
            {
                $recordString .= "Street Address 1, ";
            }

            if (recordChange($editedContact->department_street_address_2, $oldContact->department_street_address_2))
            {
                $recordString .= "Street Address 2, ";
            }

            if (recordChange($editedContact->department_street_address_3, $oldContact->department_street_address_3))
            {
                $recordString .= "Street Address 3, ";
            }

            if (recordChange($editedContact->department_country_id, $oldContact->department_country_id))
            {
                $recordString .= "Country, ";
            }

            if (recordChange($editedContact->department_provstate_id, $oldContact->department_provstate_id))
            {
                $recordString .= "Province/State, ";
            }
        
            if (recordChange($editedContact->department_region_id, $oldContact->department_region_id))
            {
                $recordString .= "Region, ";
            }

            if (recordChange($editedContact->department_city, $oldContact->department_city))
            {
                $recordString .= "City, ";
            }

            if (recordChange($editedContact->department_postal_code, $oldContact->department_postal_code))
            {
                $recordString .= "Postal Code, ";
            }
        }

		if (recordChange($editedContact->department_phone, $oldContact->department_phone))
		{
			$recordString .= "Phone, ";
		}
		
		if (recordChange($editedContact->department_fax, $oldContact->department_fax))
		{
			$recordString .= "Fax, ";
		}

		if (recordChange($editedContact->department_email, $oldContact->department_email))
		{
			$recordString .= "E-mail, ";
		}

		if (recordChange($editedContact->department_industry_id, $oldContact->department_industry_id))
		{
			$recordString .= "Industry, ";
		}

		if (recordChange($editedContact->department_size_id, $oldContact->department_size_id))
		{
			$recordString .= "Size, ";
		}
	
		if (trim($recordString))
                {
                        $recordString = substr($recordString, 0, (strlen($recordString) - 2));
                        return $recordString;
                }
                else
                {
                        return false;
                }

                break;	

	case "company":

		$oldContact = new EditContact('');
		$oldContact->loadCompanyValues($editedContact->employer_id);

		if (recordChange($editedContact->company_name, $oldContact->company_name))
		{
			$recordString .= "Name, ";
		}
	
		if (recordChange($editedContact->company_website, $oldContact->company_website))
		{
			$recordString .= "Website, ";
		}

		if (recordChange($editedContact->company_description, $oldContact->company_description))
		{
			$recordString .= "Description, ";
		}

		if (recordChange($editedContact->company_street_address_1, $oldContact->company_street_address_1))
		{
			$recordString .= "Street Address 1, ";
		}

		if (recordChange($editedContact->company_street_address_2, $oldContact->company_street_address_2))
		{
			$recordString .= "Street Address 2, ";
		}

		if (recordChange($editedContact->company_street_address_3, $oldContact->company_street_address_3))
                {
                        $recordString .= "Street Address 3, ";
                }

		if (recordChange($editedContact->company_country_id, $oldContact->company_country_id))
		{
			$recordString .= "Country, ";
		}

		if (recordChange($editedContact->company_provstate_id, $oldContact->company_provstate_id))
		{
			$recordString .= "Province/State, ";
		}
	
		if (recordChange($editedContact->company_region_id, $oldContact->company_region_id))
		{
			$recordString .= "Region, ";
		}

		if (recordChange($editedContact->company_city, $oldContact->company_city))
		{
			$recordString .= "City, ";
		}

		if (recordChange($editedContact->company_postal_code, $oldContact->company_postal_code))
		{
			$recordString .= "Postal Code, ";
		}

		if (recordChange($editedContact->company_phone, $oldContact->company_phone))
		{
			$recordString .= "Phone, ";
		}
		
		if (recordChange($editedContact->company_fax, $oldContact->company_fax))
		{
			$recordString .= "Fax, ";
		}

		if (recordChange($editedContact->company_email, $oldContact->company_email))
		{
			$recordString .= "E-mail, ";
		}

		if (recordChange($editedContact->company_industry_id, $oldContact->company_industry_id))
		{
			$recordString .= "Industry, ";
		}

		if (recordChange($editedContact->company_type_id, $oldContact->company_type_id))
		{
			$recordString .= "Company Type, ";
		}

		if (recordChange($editedContact->company_size_id, $oldContact->company_size_id))
		{
			$recordString .= "Size, ";
		}

		if (trim($recordString))
                {
                        $recordString = substr($recordString, 0, (strlen($recordString) - 2));
                        return $recordString;
                }
                else
                {
                        return false;
                }

                break;	

	} //switch

}
// }}}
// {{{ function recordChange($editedField, $oldField)

/**
 * Function below is used by recordContactChange to check if two values of the
 * contact class are equivalent, or if the change needs to be recorded.
 */
function recordChange($editedField, $oldField)
{
	if (!trim($oldField))
	{
		return false;
	}
	
	if (trim($editedField) == trim($oldField))
	{
		return false;
	}
	elseif (!trim($editedField) && !trim($oldField))
	{
		//This case should already have been caught, but just in case.

		return false;
	}
	else
	{
		return true;
	}
}
// }}}
// {{{ function check_student_name(&$students_in)

/**
 *  Description: Takes a string of student names (separated by line feed, or "," or ";" or ":", check whether
 *                               it is empty or has duplicate names, return error message; if not, return "" and correct format
 *                               of student names separated by "line feed" - chr(10)
 */
function check_student_name(&$students_in)
{
        $students = trim($students_in);
        $students = str_replace(chr(13), "", $students);                                        //remove carriage return

        if( ereg(",", $students) || ereg(":", $students) || ereg(";", $students) )
        {                                                                               //if student names are separated by "," or
                $students = str_replace( chr(10), "", $students);                       //":" or ";", remove extra line feeds
                $students = str_replace( "\\", "", $students);
                $students = preg_replace("/[,:;]/", chr(10), $students);        //add line feeds to separate student names
        }

        $students = explode(chr(10), $students);
        foreach( $students as $student )
        {
                $student = trim($student);
                $student = preg_replace("/\s{1,}/", chr(32), $student );                //remove extra white space between first name and last name
                if( $student ) { $temp_students[] = $student;  }
        }
        if( empty($temp_students) )
        {
                return "You must have at least one student short-listed";
        }
        else
        {
                $students = implode( $temp_students, chr(10) );
                $students_in = $students;
		return NULL;
        }
}
// }}}
// {{{ function update_duplicate($select)

/**
 * Description: check if there is duplicate student names left undecided.
 *                              If so, display their student numbers to let admin to
 *                              decide which one is supposed to have interview.
 */
function update_duplicate($select)
{
        global $job_id;
        global $job_code;
        global $last_modified;

        global $name_array_run;

        global $sid_array;

        global $student_num;
        global $duplicate_students;
        global $duplicate;

        global $conflict_studentnum;
        global $conflict_studentname;
        global $conflict_studentemail;
        global $noemail_studentnum;
        global $noemail_studentname;
        global $sendEmail;
        global $post;

        global $PHP_SELF;
        global $URL;
        global $coming_through_again;
        global $interview;

        global $auth;

        if( $student_num )
        {
                $sid_array[] = $student_num;
        }

        if( $duplicate_students && count($duplicate_students) >0 )
        {
		$student_nums = array_shift( $duplicate_students );

                $temp_array[] = array("select", $select);
                $temp_array[] = array("job_id", $job_id);
                $temp_array[] = array("job_code", $job_code);
                $temp_array[] = array("last_modified", $last_modified);

                $temp_array = name_value_pair("duplicate_students", $duplicate_students, $temp_array);
                $temp_array = name_value_pair("sid_array", $sid_array, $temp_array);

                //select student number for duplicate student name
                if (!$coming_through_again)
                {
                        echo("<br />");
                }
                $name_array_run = true;
                select_name_array($student_nums, $PHP_SELF, $temp_array);
        }
        else
        {


                if( $select == "edit" )
                {
                        //get lock
                        $sql = "SELECT GET_LOCK('$job_id', 10)";
                        $row = $GLOBALS['dbh']->GetRow($sql);
                        if ($row[0] != "1"){
                                $interview = new EditInterview('');
                                $interview->loadValues($job_id);
                                for($i=0;$i<count($sid_array);$i++){
                                        $student_str .= getStudentName($sid_array[$i]) . chr(13);
                                }
                                chop($student_str);
                                $interview->students = $student_str;
                                error ("Can't get record lock.  Please Try again.");
                                include ("interview/edit/edit_students_form.inc");
                                return;
                        }
                        //retrieve previous shortlisted students' student number, then clear short list

                        $result19a = $GLOBALS['dbh']->Execute("SELECT student_number, last_visited
                                                from students_shortlisted WHERE job_id='$job_id'");

                        while( ($row19a = $result19a->FetchRow() ))
                        {
                                $old_sid_array[] = $row19a["student_number"];
                                $old_last_visited[] = $row19a["last_visited"];
                        }


                        $sql = "SELECT MAX(date_edited) as last_modified FROM interview_edit
                                WHERE job_id = '$job_id' AND menu_edited = 'Students Shortlisted'";
                        $row = $GLOBALS['dbh']->GetRow($sql);
                        if ($row["last_modified"] != "" && $row["last_modified"] > $last_modified){
                                $sql = "SELECT CONCAT(first_name, ' ', last_name) as name
                                        FROM contact as c, contact_internal as a,  interview_edit as i
                                        WHERE a.contact_id = c.contact_id AND a.login_id = i.editor AND
                                                i.job_id ='$job_id' AND menu_edited = 'Students Shortlisted'
                                                AND date_edited = '" . $row["last_modified"] . "'";
                                $row = $GLOBALS['dbh']->GetRow($sql);
                                if ($row["name"] == ""){
					$name = "An unknown person";
                                }else{
                                        $name = $row["name"];
                                }

                                $edit_conflict = 1;
                                $interview = new EditInterview('');
                                $interview->loadValues($job_id);
                                $GLOBALS['dbh']->GetRow("SELECT RELEASE_LOCK('$job_id')");

                                error ($name . " has been editing the shortlist for this interview at the same time as you.  Your
                                                changes have NOT been saved, but are shown below in bold.
                                                Enter the correct shortlist and then save the changes.");

                                include ("interview/edit/edit_students_form.inc");

                                return;
                        }

                }

                $result19b = $GLOBALS['dbh']->Execute("DELETE FROM students_shortlisted WHERE job_id='$job_id'");

                if( count($sid_array) >= 1 )
                {
                        foreach( $sid_array as $student_number )
                        {
                                $result = $GLOBALS['dbh']->Execute("SELECT student_number FROM students_shortlisted
                                                        WHERE student_number='$student_number' AND job_id='$job_id'");
                                if( $result->RecordCount() == 0 )
                                {
                                        $GLOBALS['dbh']->Execute( "INSERT INTO students_shortlisted (job_id, student_number) VALUES ('$job_id', '$student_number')" );
                                }
                        }
                }//else

                if( $select == "edit")
                {
                        /**
                         * if a student who already signed up for this interview is removed from student list, update the
                         * interview time table to free the corresponding timeslot.
                         */

                        for($i = 0; $i < count( $old_sid_array ); $i++)
                        {
                                $match = 0;

                                if( in_array( $old_sid_array[$i], $sid_array ) ) { $match = 1; }

                                if( $match == 0 )
                                        //no match, remove from interview time table if this student already signed up for this interview
                                        $GLOBALS['dbh']->Execute("      UPDATE interview_time
                                                                SET student_number=null
                                                                WHERE job_id='$job_id' AND student_number='$old_sid_array[$i]'");

                                elseif( $old_last_visited[$i])
                                        $GLOBALS['dbh']->Execute("      UPDATE students_shortlisted
                                                                SET last_visited = '" . $old_last_visited[$i] . "'
                                                                WHERE job_id='$job_id' AND student_number='$old_sid_array[$i]'" );

                        }
			if ($name_array_run || $duplicate)
                        {
                                echo("
                                        <table>
                                        <tr>
                                        <td>
                                        <h3>Edit Interview: " . $job_code . " </h3>
                                        </td>
                                        ");
                                echo("
                                        <td>
                                        <form name='myform' method='POST' action='$PHP_SELF&PDF=1&job_id=$job_id'>
                                        <select name='selection'>
                                            <option value='sign_up'>Sign up Students for this Interview
                                            <option value='single'>Preview Schedule
                                            <option value='final'>Final Interview Schedule
                                            <option value='email_schedule'>E-mail Schedule to Employer
                                        </select>
                                        &nbsp;
                                        <input type='image' src='misc/images/arrowg.gif' alt='go'>
                                        </form>
                                        </td>
                                        </tr>
                                        </table>
                                ");



                        }

                        //changes to shortlist have been made
                        $result = $GLOBALS['dbh']->Execute("REPLACE INTO interview_edit(job_id, date_edited, editor, menu_edited)
                                                VALUES('" . $job_id . "', NULL, '" . $auth->login . "', 'Students Shortlisted')");

                        $GLOBALS['dbh']->GetRow("SELECT RELEASE_LOCK('$job_id')");

                        if ($sendEmail)
                        {
                                error("Changes to shortlist for " . $job_code . " have been saved.  It is important that you send this e-mail to any students that have been dropped from interviews they were previously signed up for.");
                                include('interview/edit/mail/send_students_email.inc');
                        }
                        else
                        {
                                notify("Change to shortlist for " . $job_code . " have been saved.");
                                $interview = new EditInterview('');
                                $interview->loadValues($job_id, 'Students');
                                include('interview/edit/edit_students_form.inc');
                        }
                }
                else
                {
                        notify($job_code." added");
                        echo("<br />");
                        if ($post)
                        {
                                include('interview/add/send_newsgroup_email.inc');
                        } else
                        {
                                include('interview/add/show_data.inc');
                        }
                }
        }
}
// }}}
// {{{ function name_value_pair($src_name, $src_array, $tar_array)

function name_value_pair($src_name, $src_array, $tar_array)
{
        for($i = 0; $i < count($src_array); $i++ )
        {
                if( is_array($src_array[$i]) )
                        for( $j = 0; $j < count($src_array[$i]); $j++ )
                                $tar_array[] = array("$src_name"."[$i][$j]", $src_array[$i][$j]);

                else
                        $tar_array[] = array("$src_name"."[$i]", $src_array[$i]);
        }
        return $tar_array;
}
// }}}
// {{{ function sortName(&$names)

/**
 * Sort the given names by last name, if duplicate last names exist, sort duplicate ones by first name.
 */
function sortName( &$names )
{
        assert( is_array( $names ) );
        if( count( $names ) <= 1 )
                return;

        if( is_string( $names[0] ) )
        {
                for( $i=0; $i<count($names); $i++ )
                {
                        $temp_array = explode(chr(32), $names[$i]);
                    if( count($temp_array) == 3 )
                                /***  ignore initials of student names  ***/
                                $names[$i] = array( $temp_array[0], $temp_array[2] );
                        else
                                $names[$i] = array( $temp_array[0], $temp_array[1] );
                }
                usort( $names, "compareName" );
                for( $i=0; $i<count( $names ); $i++ )
                {
                        $names[$i] = $names[$i][0] . chr(32) . $names[$i][1];
                }
        }
        elseif( is_array( $names[0] ) )
        {
                usort( $names, "compareName" );
        }
}
// }}}
// {{{ function compareName($left, $right)

/**
 * Comparator function for sorting names
 */
function compareName($left, $right)
{
        if( $left[1] == $right[1] )
        {
                return ( strcmp( $left[0], $right[0] ) );
        }
        return strcmp( $left[1], $right[1] );
}
// }}}
// {{{ function displayFormElement()

/**
 * Display all the received variables from HTTP POST Action
 */
function displayFormElement()
{
        global $HTTP_POST_VARS;

        foreach( $HTTP_POST_VARS as $var=>$value )
        {
                echo("$var = $value<br />");
        }
}
// }}}
// {{{ function isSlotsEnough($schedules, $students, $sw_or_sim = null)
/**
 * Check if there are enough interview slots based on the number of shortlisted students.
 * @return int  >0 => surplus
 *               0 => equal
 *              <0 => not enough
 */
function isSlotsEnough($schedules, $students,$sw_or_sim = null)
{
        if( is_string( $students ) )
                $students = count( explode( chr(10), $students ) );

        elseif( is_array( $students ) )
                $students = count( $students );

        $count;
        for( $i = 0; $i < count($schedules); $i++ )
                for($j = 0; $j < count($schedules[$i]); $j++)
                        if( $schedules[$i][$j]["TYPE"] == INTERVIEW )
                                $count++;

        if( $sw_or_sim )
                $count = $count * 2;

        return $count - $students;
}
// }}}
// {{{ function findStudentNumbers($name)

/**
 * Find all possible student number according to given full name.
 */
function findStudentNumbers($name)
{
          $temp_array = explode(chr(32), $name);

      for( $j = 1; $j < count( $temp_array ); $j++ )
	{
                $first_name = trim( implode( chr(32), array_slice($temp_array, 0, $j ) ) );
                $last_name = trim( implode( chr(32), array_slice($temp_array, $j, count($temp_array) - $j ) ) );

        $sql12 = ("
                SELECT student_number
                FROM student
                WHERE first_name='" . addslashes( stripslashes($first_name) ) . "'
                AND last_name='" . addslashes( stripslashes($last_name) ) . "'
                ");
        $result12 = $GLOBALS['dbh']->Execute($sql12);

                while( ($row12 = $result12->FetchRow()) )
                {
                        $studentNum[] = $row12["student_number"];
                }

      }//end FOR

      return $studentNum;

}
// }}}
// {{{ function getNextTermYear()

/**
 * Function below returns the NEXT term_id, and associated year, in an array.
 * @return array
 */
function getNextTermYear()
{
        /*
         The first thing that needs to be done is that we find the maximum term number.  To do this we simply
         pull out all of the term_id's, ordered from greatest to smallest, and take the first one.
        */

        $sql = ("
                SELECT DISTINCT term_id
                FROM term
                ORDER BY term_id DESC
                ");
        $max_result = $GLOBALS['dbh']->Execute($sql);
        $max_row = $max_result->FetchRow();

        $nextTerm = getCurrentTermID();
        $nextTerm++;
        if ($nextTerm > $max_row["term_id"])
        {
                $nextTerm -= $max_row["term_id"];
        }

        $sql = ("
                SELECT CURRENT_DATE
                ");
        $date_result = $GLOBALS['dbh']->Execute($sql);
        $date_row = $date_result->FetchRow();

        $current_date = explode("-", $date_row["CURRENT_DATE"]);
        $cur_year = $current_date[0];

        /*
         If the term we're returning has a starting date that is in the new year, we need to increment the year we've
         just gotten.
        */

        $sql = ("
                SELECT DISTINCT start_date, end_date
                FROM term
                WHERE term_id='" . $nextTerm . "'
                ");

        $term_result = $GLOBALS['dbh']->Execute($sql);
        $term_row = $term_result->FetchRow();

        $start = $term_row["start_date"];
        $end = $term_row["end_date"];
        $length = $end - $start;

        /*
         Now subtract the length of the term from the starting date.  If we end up with a negative number or 0,
         then we know that this is the first term to START in the new year, so we need to increment the year.
        */

        if (($start - $length) < 1)
        {
                $cur_year++;
        }

        // We now have both values we need.  Return them.

        $nextTermYear = array(
                "term" => $nextTerm,
                "year" => $cur_year
                );

        return $nextTermYear;
}
// }}}
// {{{ function getNextTermYear2($term_id, $year)

/**
 * Function below overloads the getNextTermYear() function, allowing a current term and year to be passed.
 * $order can be either 'year' or 'academic' which will determine the order that terms are progressed upwards through.
 */
function getNextTermYear2($term_id, $year)
{
        /*
         The first thing that needs to be done is that we find the maximum term number.  To do this we simply
         pull out all of the term_id's, ordered from greatest to smallest, and take the first one.
        */

        $sql = ("
                SELECT DISTINCT term_id
                FROM term
                ORDER BY term_id DESC
                ");
        $max_result = $GLOBALS['dbh']->Execute($sql);
        $max_row = $max_result->FetchRow();

        $nextTerm = $term_id;
        $nextTerm++;
        if ($nextTerm > $max_row["term_id"])
        {
                $nextTerm -= $max_row["term_id"];
        }

        $nextYear = $year;

        /*
         If the term we're returning has a starting date that is in the new year, we need to increment the year we've
         just gotten.
        */

        $sql = ("
                SELECT DISTINCT start_date, end_date
                FROM term
                WHERE term_id='" . $nextTerm . "'
                ");

        $term_result = $GLOBALS['dbh']->Execute($sql);
        $term_row = $term_result->FetchRow();

        $start = $term_row["start_date"];
        $end = $term_row["end_date"];
        $length = $end - $start;

        /*
         Now subtract the length of the term from the starting date.  If we end up with a negative number or 0,
         then we know that this is the first term to START in the new year, so we need to increment the year.
        */

        if (($start - $length) < 1)
        {
                $nextYear++;
        }

        // We now have both values we need.  Return them.

        $nextTermYear = array(
                "term" => $nextTerm,
                "year" => $nextYear
                );

        return $nextTermYear;

}
// }}}
// {{{ function getPreviousTermYear()

/**
 * Function below returns the PREVIOUS term_id, and associated year, in an array.
 */
function getPreviousTermYear()
{

	$sql = ("
		SELECT DISTINCT term_id
		FROM term
		ORDER BY term_id
	");
	$min_result = $GLOBALS['dbh']->Execute($sql);
	$min_row = $min_result->FetchRow();

	$prevTerm = getCurrentTermID();
	$prevTerm--;
	if ($prevTerm < $min_row["term_id"])
	{
		$prevTerm += $min_row["term_id"];
	}

	$sql = ("
		SELECT CURRENT_DATE
	");
	$date_result = $GLOBALS['dbh']->Execute($sql);
	$date_row = $date_result->FetchRow();

	$current_date = explode("-", $date_row["CURRENT_DATE"]);
	$cur_year = $current_date[0];

	$sql = ("
		SELECT DISTINCT start_date, end_date
		FROM term
		WHERE term_id='" . $prevTerm . "'
	");

	$term_result = $GLOBALS['dbh']->Execute($sql);
	$term_row = $term_result->FetchRow();

	$start = $term_row["start_date"];
	$end = $term_row["end_date"];
	$length = $end - $start;

	if (($end + $length) > 12)
	{
		$cur_year--;
	}

	$prevTermYear = array(
			"term" => $prevTerm,
			"year" => $cur_year
			);

	return $prevTermYear;

}
// }}}
// {{{ function getCurrentTermID()
/**
 * Function below get's the current term_id from the database.
 */
function getCurrentTermID()
{
        $sql = ("
                SELECT DISTINCT CURRENT_DATE
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $current_date = explode('-', $row["CURRENT_DATE"]);

        $sql = ("
                SELECT DISTINCT term_id
                FROM term
                WHERE start_date<='" . $current_date[1] . "'
                AND end_date>='" . $current_date[1] . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $getCurrentTermID = $row["term_id"];
        return $getCurrentTermID;
}
// }}}
// {{{ function getTermYearWithGracePeriod($date)

/**
 * @param string $date YYYY-MM-DD format
 * @return array An array with term and year as associative keys
 */
function getTermYearWithGracePeriod($date)
{
    $arr_current_start_date = explode("-",$date);
    $current_term = getTerm($arr_current_start_date[1]);
    $current_year = $arr_current_start_date[0];

    // temporarily change start date to +HISTORY_TERM_ROLLOVER_GRACE_PERIOD days, and see what term we end up in.
    // We add a grace period to the date to see if the date is close to a term roll over point. For example, for
    // Spring, the last day of the spring term is April 30. The summer term starts at May 1st. If an employer puts
    // in a job that starts on April 25, it technically is a summer job since it's so close to summer. So we use
    // the below to roll over the term to the next term for cases like this.
    $temp_start_date = date("Y-m-d",strtotime("+".HISTORY_TERM_ROLLOVER_GRACE_PERIOD." days",strtotime($date)));
    $arr_temp_start_date = explode("-",$temp_start_date);

    $arr_term_year = array();

    // If the date spills over to next term, then grab next term
    if (getTerm($arr_temp_start_date[1]) != $current_term)
    {
        $arr_term_year = getNextTermYear2($current_term, $current_year);
    }
    // Otherwise, use the current term. 
    else
    {
        $arr_term_year["term"] = $current_term;
        $arr_term_year["year"] = $current_year;
    }

    return $arr_term_year;
}
// }}}
// {{{ function getTermYearEndDate($term_id, $year)

/**
 * Figures out the last day of the term.
 * @param int $term_id The term ID.
 * @param int $year The year.
 * @return string $end_date or -1 if an error occurs. 
 */
function getTermYearEndDate($term_id, $year)
{
    // We will grab the first date of next term, and subtract one day from it, and voila, we have the end date
    // of this term. 
    $arr_next_term_year = getNextTermYear2($term_id, $year);
    $next_term_id = $arr_next_term_year['term'];
    
    $sql = ("
        SELECT start_date
        FROM term
        WHERE term_id = '".$next_term_id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount())
    {
        $row = $result->FetchRow();
        // padding start date, eg: 1 becomes 01. 11 becomes 11.  
        $row['start_date'] = str_pad($row['start_date'],2,0,STR_PAD_LEFT);
        // making a YYYY-MM-DD formated date
        $temp_end_date = $arr_next_term_year['year']."-".$row['start_date']."-01";
        $end_date = date("Y-m-d",strtotime("-1 days",strtotime($temp_end_date)));

        return $end_date;
    }
    else
    {
        return -1;
    }
}
// }}}
// {{{ function isApplicableDept($job_id, $department_id)

/**
 * Function below takes a job and a department_id, and determines if the given department_id is one
 * that can apply to this job.
 */
function isApplicableDept($job_id, $department_id)
{
	$departments_in_group = department_groups($department_id);
	if ($departments_in_group)
	{
		foreach($departments_in_group AS $dig)
		{
			$dept_ids = $dept_ids."'" . $dig . "',";
		}
	}

	if ($dept_ids)
	{
		$dept_ids = substr($dept_ids, 0, strlen($dept_ids)-1);
	}
	else
	{
		$dept_ids =("''");
	}

	$sql = ("
		SELECT DISTINCT department_id
		FROM department_job_join
		WHERE job_id='" . $job_id . "'
		AND department_id IN (" . $dept_ids . ")
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	$isApplicableDept = $result->RecordCount();

	return($isApplicableDept);
}
// }}}
// {{{ function canStudentApply($job_id, $student_number)

/**
 * Function below determines if a student can apply to a job, given a job_id and student_number
 * A student can apply if:
 * - The job is applicable to their department.
 * - The job is not closed.
 * - The student has not already applied to the job
 * - The job requires paper applications
 * - The student is not eligible to go on a workterm.
 */

function canStudentApply($job_id, $student_number)
{
	// Check that the student hasn't already applied.

	$sql = ("
		SELECT DISTINCT application_id
		FROM applications
		WHERE student_number='" . $student_number . "'
		AND job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if ($result->RecordCount())
	{
		return(false);
	}

	// Check that the student is in a department applicable to this job.

	$sql = ("
		SELECT DISTINCT department_id
		FROM student_department
		WHERE student_number='" . $student_number . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	$applicableDept = FALSE;

	while ($row = $result->FetchRow())
	{
		if (isApplicableDept($job_id, $row["department_id"]))
		{
			$applicableDept = TRUE;
			break;
		}
	}

	if (!$applicableDept)
	{
		return(false);
	}


	// Check that the job is accepting online applications or is special

	$sql = ("
		SELECT DISTINCT apply_method
		FROM job_info	
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	if (($row["apply_method"] == JOB_APP_METHOD_PAPER) || ($row["apply_method"] == JOB_APP_METHOD_SPECIAL))
	{
		return(false);
	}

	// Check that the job is not closed.

	$sql = ("
		SELECT DISTINCT status
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();

	if ($row["status"] != POSTED)
	{
		return(false);
	}

	// Check that the student is eligible for a workterm.

	$sql = ("
		SELECT DISTINCT term_id, year
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	$sql = ("
		SELECT DISTINCT record_id
		FROM student_department
		WHERE student_number='" . $student_number . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($record_row = $result->FetchRow())
	{
		$sql = ("
			SELECT DISTINCT record_id
			FROM eligible_placed
			WHERE record_id='" . $record_row["record_id"] . "'
			AND term_id='" . $row["term_id"] . "'
			AND year='" . $row["year"] . "'
			AND student_flags_id='" . ELIGIBLE_FLAG . "'
			");
		$flag_result = $GLOBALS['dbh']->Execute($sql);
		while ($record_row = $flag_result->FetchRow())
		{
			$record_ids .= ("'" . $record_row["record_id"] . "',");
		}
	}
	
	if (!$record_ids)
	{
		return(false);
	}
	else
	{
		// Now we have a list of departments this student is eligible for.
		$record_ids = substr($record_ids, 0, -1);
	}

	// Now take the record ids we just got and see if this job is applicable to any of them.

	$sql = ("
		SELECT DISTINCT department_id
		FROM student_department
		WHERE record_id IN (" . $record_ids . ")
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($row = $result->FetchRow())
	{
		$depts_in_group = department_groups($row["department_id"]);
		foreach($depts_in_group AS $dig)
		{
			$dept_ids .= ("'" . $dig . "',");
		}
	}
	$dept_ids = substr($dept_ids, 0, -1);
			
	$sql = ("
		SELECT DISTINCT department_id
		FROM department_job_join
		WHERE job_id='" . $job_id . "'
		AND department_id IN (" . $dept_ids . ")
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if (!$result->RecordCount())
	{
		return(false);
	}

	// We've made it this far, so the student is eligible.
	return(true);
}
// }}}
// {{{ function reasonStudentCantApply($job_id, $student_number)

/**
 * Function below determines why the student cannot apply to a given job, and returns 
 * a string stating the case.
 */
function reasonStudentCantApply($job_id, $student_number)
{
	// Check that the student hasn't already applied.

	$sql = ("
		SELECT DISTINCT application_id
		FROM applications
		WHERE student_number='" . $student_number . "'
		AND job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if ($result->RecordCount())
	{
		return("already_applied");
	}

	// Check that the student is in a department applicable to this job.

	$sql = ("
		SELECT DISTINCT department_id
		FROM student_department
		WHERE student_number='" . $student_number . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	$applicableDept = FALSE;

	while ($row = $result->FetchRow())
	{
		if (isApplicableDept($job_id, $row["department_id"]))
		{
			$applicableDept = TRUE;
			break;
		}
	}

	if (!$applicableDept)
	{
		return("dept_not_applicable");
	}

	// Check that the job is accepting online applications or has special method

	$sql = ("
		SELECT DISTINCT apply_method
		FROM job_info	
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	if ($row["apply_method"] == JOB_APP_METHOD_PAPER)
	{
		return JOB_APP_METHOD_PAPER;
	}
	elseif ($row["apply_method"] == JOB_APP_METHOD_SPECIAL)
	{
		return JOB_APP_METHOD_SPECIAL;
	}

	// Check that the job is not closed.

	$sql = ("
		SELECT DISTINCT status
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();

	if ($row["status"] != '3')
	{
		return("job_not_posted");
	}

	// Check that the student is eligible for a workterm.

	$sql = ("
		SELECT DISTINCT term_id, year
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	$sql = ("
		SELECT DISTINCT record_id
		FROM student_department
		WHERE student_number='" . $student_number . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($record_row = $result->FetchRow())
	{
		$sql = ("
			SELECT DISTINCT record_id
			FROM eligible_placed
			WHERE record_id='" . $record_row["record_id"] . "'
			AND term_id='" . $row["term_id"] . "'
			AND year='" . $row["year"] . "'
			AND student_flags_id='" . ELIGIBLE_FLAG . "'
			");
		$flag_result = $GLOBALS['dbh']->Execute($sql);
		while ($record_row = $flag_result->FetchRow())
		{
			$record_ids .= ("'" . $record_row["record_id"] . "',");
		}
	}
	
	if (!$record_ids)
	{
        //print $sql;
		return("not_eligible");
	}
	else
	{
		// Now we have a list of departments this student is eligible for.
		$record_ids = substr($record_ids, 0, -1);
	}

	// Now take the record ids we just got and see if this job is applicable to any of them.

	$sql = ("
        SELECT DISTINCT department_id
        FROM student_department
        WHERE record_id IN (" . $record_ids . ")
        ");
    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow())
    {
        $depts_in_group = department_groups($row["department_id"]);
        foreach($depts_in_group AS $dig)
        {
            $dept_ids .= ("'" . $dig . "',");
        }
    }
    $dept_ids = substr($dept_ids, 0, -1);

    $sql = ("
            SELECT DISTINCT department_id
            FROM department_job_join
            WHERE job_id='" . $job_id . "'
            AND department_id IN (" . $dept_ids . ")
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

	if (!$result->RecordCount())
	{
		return("not_eligible");
	}
	
	/*
	 We should never make it this far, since this function should
	 only be called if the student CAN'T apply.
	*/
	
	return("can_apply");
}
// }}}
// {{{ function generatePassword($size = 8)

/**
 * Function below generates a password of specified length.  The password will contain only 
 * alphanumeric characters, so essentially the letters of the alphabet (lower and upper case)
 * as well as the digits 0-9.  The default size of the password is 8, but a different length
 * can be specified if desired.
 */
function generatePassword($size = 8)
{
	$count = 0;

	while ($count < $size)
	{
		while (!(isset($ascii_num) && ($ascii_num != 79 && $ascii_num != 108) && ($ascii_num < 58 || 
		($ascii_num > 64 && $ascii_num < 91) || $ascii_num > 96)))
		{
			$ascii_num = rand(50,122);
		}
	 
		$password_str .= chr($ascii_num);
		unset($ascii_num);
		$count++;
	}
	
	return $password_str;
}
// }}}
// {{{ function popup($fieldname,$formname)

/**
 * Function to include popup calendar feature if it is an installed component in the system
 */
function popup($fieldname,$formname)
{
	$sql = ("
        SELECT installed 
        FROM installed_components 
        WHERE description='calendar'
        ");
	$results=$GLOBALS['dbh']->Execute($sql);
	$row=$results->FetchRow();	
    
    if($row['installed'] == 1)
    {
		return "<img src='misc/images/calendar_popup.gif' onmousedown=\"popup('".$fieldname."', '".$formname."')\" alt='' />";	
	}
    else
    {
		return NULL;
	}
}
// }}}
// {{{ function is_installed($description)

/**
 * Function that returns a boolean stating whether a certain report button is an installable component in the system
 */
function is_installed($description)
{
    $sql = ("
        SELECT installed 
        FROM installed_components
        WHERE description = '" .$description. "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    
    while( $row = $result->FetchRow())
    {
        if($row['installed'] == 1)
        { 
            return true;
        }
        else
        {
            return false;
        }
    }
}
// }}}
// {{{ function further_progress($job_id, $student_number)

/**
 * Function below is used to determine whether the student is able to continue their
 * progression in this job, or if that is no longer possible.
 */
function further_progress($job_id, $student_number)
{
	// If this is a multi-job interview, we need the interviews job_id as well
	$sql = ("
		SELECT interview_job_id
		FROM interview_job_join
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if ($row = $result->FetchRow())
	{
		$interview_job_id = $row["interview_job_id"];
	}
	else
	{
		$interview_job_id = $job_id;
	}

	$sql = ("
		SELECT status
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();

	if ($row["status"] == POSTED)
	{
		return(true);
	}
	elseif ($row["status"] == CLOSED || $row["status"] == CANCELLED)
	{
		// The job is closed, has the student applied for it?

		$sql = ("
			SELECT DISTINCT application_id
			FROM applications
			WHERE job_id='" . $job_id . "'
			AND student_number='" . $student_number . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		if (!$result->RecordCount())
		{
			return(true);
		}

		// They have applied for it.  Is the Employer still hiring?
		$sql = ("
			SELECT student_status
			FROM job_info
			WHERE job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		if ($row["student_status"] == STUDENT_NOT_HIRING)
		{
			// Employer not hiring, so further progress is not possible
			return(false);
		}

		// Is there a shortlist out for this job?

		$sql = ("
			SELECT job_id
			FROM students_shortlisted
			WHERE job_id='" . $interview_job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		
		if (!$result->RecordCount())
		{
			// They haven't shortlisted yet, so further progress is possible
			return(true);
		}

		// There is a shortlist for this job.  Is the student on it?

		$sql = ("
			SELECT student_number
			FROM students_shortlisted
			WHERE job_id='" . $interview_job_id . "'
			AND student_number='" . $student_number . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		if (!$result->RecordCount())
		{
			// Student hasn't been shortlisted.
			return(false);
		}

		// Is there a placement for this job?

		$sql = ("
			SELECT job_id
			FROM history
			WHERE job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		if (!$result->RecordCount())
		{
			// No placement yet, so progress still possible.
			return(true);
		}
		
		// Is the placement for this student?

		$sql = ("
			SELECT student_number
			FROM history
			WHERE job_id='" . $job_id . "'
			AND student_number='" . $student_number . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		
		if ($result->RecordCount())
		{
			// Yes, this student is placed in this job.
			return(true);
		}
		else
		{
			// Nope, some other student got the job.
			return(false);
		}
	}
}
// }}}
// {{{ function employer_not_hiring($job_id)

/**
 * Function below returns true if the employer is not hiring.
 */
function employer_not_hiring($job_id)
{
	$sql = ("
		SELECT student_status
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	if ($row["student_status"] == STUDENT_NOT_HIRING)
	{
		// Employer not hiring
		return(true);
	} else {
		return(false);
	}
}
// }}}
// {{{ function is_job_flagged($job_id, $student_number)

/**
 * Function below returns true if the student has flagged the given job.
 */
function is_job_flagged($job_id, $student_number)
{
	$sql = ("
		SELECT DISTINCT job_id
		FROM student_jobs_flagged
		WHERE job_id='" . $job_id . "'
		AND student_number='" . $student_number . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if ($result->RecordCount())
	{
		return(true);
	}
	else
	{
		return(false);
	}
}
// }}}
// {{{ function getJobCode($job_id)

/**
 * Get the job code for a job_id
 */
function getJobCode($job_id)
{
	$sql = ("
		SELECT DISTINCT job_code
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$job_code = $row["job_code"];
	
	return($job_code);
}
// }}}
// {{{ function getJobEmployer($job_id)

/**
 * Get the employer_id for a given job
 */
function getJobEmployer($job_id)
{
	$sql = ("
		SELECT DISTINCT employer_id
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$employer_id = $row["employer_id"];

	return($employer_id);
}
// }}}
// {{{ function hasStudentApplied($job_id, $student_number)

/**
 * Has the student applied to this job?
 */
function hasStudentApplied($job_id, $student_number)
{
	$sql = ("
		SELECT DISTINCT application_id
		FROM applications
		WHERE job_id='" . $job_id . "'
		AND student_number='" . $student_number . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if ($result->RecordCount())
	{
		return true;
	}
	else
	{
		return false;
	}
}
// }}}
// {{{ function isAdminPulled($job_id, $student_number)

/**
 * Has the student's application been pulled?
 */
function isAdminPulled($job_id, $student_number)
{
	$sql = ("
		SELECT DISTINCT application_id
		FROM applications
		WHERE job_id='" . $job_id . "'
		AND student_number='" . $student_number . "'
		AND added_by_admin='1'
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount())
	{
		return true;
	}
	else
	{
		return false;
	}

}
// }}}
// {{{ function hasStudentBeenPlacedInJob($job_id, $student_number)

/**
 * Has student been placed in this job?
 */
function hasStudentBeenPlacedInJob($job_id, $student_number)
{
	$sql = ("
		SELECT DISTINCT history_id
		FROM history
		WHERE job_id='" . $job_id . "'
		AND student_number='" . $student_number . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if ($result->RecordCount())
	{
		return true;
	}
	else
	{
		return false;
	}
}
// }}}
// {{{ function getDeptNameArray($job_id)

/**
 * Get the string of departments that are associated with this job
 */
function getDeptNameArray($job_id)
{
    $sql = ("
        SELECT DISTINCT d.department_name
        FROM department_job_join djj
        INNER JOIN department d ON djj.department_id = d.department_id
        WHERE djj.job_id = ".$job_id
        );
    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow())
    {
        $deptArray[] = $row[department_name];
    }

    return $deptArray;
}
// }}}
// {{{ function getDeptIDArray($job_id)

/**
 * Get the string of departments that are associated with this job
 */
function getDeptIDArray($job_id)
{
    $sql = ("
        SELECT DISTINCT department_id
        FROM department_job_join
        WHERE job_id = ".$job_id
        );
    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow())
    {
        $deptIDArray[] = $row[department_id];
    }

    return $deptIDArray;
}
// }}}
// {{{ function getDeptCodeArray($job_id)

/**
 * Get the string of departments that are associated with this job
 */
function getDeptCodeArray($job_id)
{
    $sql = ("
        SELECT DISTINCT department_id
        FROM department_job_join
        WHERE job_id = ".$job_id
        );
    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow())
    {
        $deptCodeArray[] = getDepartmentCode($row[department_id]);
    }

    return $deptCodeArray;
}
// }}}
// {{{ function getDiscArray($job_id)

/**
 * Get the string of disciplines that are associated with this job
 */
function getDiscArray($job_id)
{
	$sql = ("
		SELECT DISTINCT dd.discipline_code
		FROM department_disciplines AS dd, job_discipline_join AS jdj
		WHERE jdj.job_id='" . $job_id . "'
		AND dd.discipline_id=jdj.discipline_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if (!$result->RecordCount())
	{
		/*
		 There is nothing in the table for this job.  Update database
		 so this job has the ALL_DISCIPLINES tag marked.
		*/

		$sql = ("
			INSERT INTO job_discipline_join
			(job_id, discipline_id)
			VALUES (
			'" . $job_id . "',
			'" . ALL_DISCIPLINES . "'
			)
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		
		$sql = ("
			SELECT DISTINCT discipline_code
			FROM department_disciplines
			WHERE discipline_id='" . ALL_DISCIPLINES . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$disc_array[] = $row["discipline_code"];
	}
	else
	{
		while ($row = $result->FetchRow())
		{
			$disc_array[] = $row["discipline_code"];
		}
	}

	return ($disc_array);
}
// }}}
// {{{ function getApplicantRange($total_apps)

/*
 * Name: getApplicantRange
 * @param int @total_apps Number of applicants for a given job 
 * @return string A number range based on the number of applicants for a given job id
 */
function getApplicantRange($total_apps)
{
    //Check assigns a range to the retrieved number
    if($total_apps > 20) {
        $applicant_range = "21+";
    }
    elseif($total_apps <= 20 && $total_apps > 10) {
        $applicant_range = "11-20";
    }
    elseif($total_apps <= 10 && $total_apps > 5) {
        $applicant_range = "6-10";                                                                                                               
    }
    elseif($total_apps <= 5 && $total_apps > 0) {                                                                                                 
        $applicant_range = "1-5";                                                                                                                
    }
    elseif($total_apps == 0) { 
        $applicant_range = "0";                                                                                                                  
    }
    else {
        $applicant_range = "n/a";
    }

    return $applicant_range;
}  
// }}}
// {{{ function pulldownGroup($departments_in_group)

/**
 * This function returns 0 if the user has their preferences set to using group pulldown menus, otherwise it returns their department 
 */
function pulldownGroup($departments_in_group)
{
global $auth;

	if ($department_id == ""){
		if (sizeof($departments_in_group) > 1){
			if ($_SESSION['SESSION_netlog']==SCRIPT_LOGIN) {
				$group_sql = "SELECT b.pulldown_menu_group
						FROM contact_internal AS b
						WHERE b.contact_id='$auth->contact_id'";
			} else {
				$group_sql = "SELECT b.pulldown_menu_group
						FROM contact_internal AS b
						WHERE b.contact_id='$auth->contact_id'";
			}
			$result_group=$GLOBALS['dbh']->Execute($group_sql);
			while($row=$result_group->FetchRow()){
				$pulldown_menu_group=$row['pulldown_menu_group'];
			}
			if ($pulldown_menu_group=='1'){
				return('0');
			} else {
				return($auth->department);
			}
		} else {
			return($auth->department);
		}
	}
}
// }}}
// {{{ function getAdminStatusGifArray()

/**
 * This function returns an associative array that contains the number and gif for each admin status.
 */
function getAdminStatusGifArray()
{
	$gif_array = Array(
		Array(
			"name" => "Not Checked - Employer entered job",
			"gif" => "<img border='0' src='misc/images/not_checked_employer.gif' alt='' />"
		),
		Array(
			"name" => "Not Checked - Staff entered job",
			"gif" => "<img border='0' src='misc/images/not_checked_staff.gif' alt='' />"
		),
		Array(
			"name" => "Checked - Job needs to be coded",
			"gif" => "<img border='0' src='misc/images/checked_needs_to_be_coded.gif' alt='' />"
		),
		Array(
			"name" => "Checked - Ready to post but awaiting date",
			"gif" => "<img border='0' src='misc/images/checked_awaiting_date.gif' alt='' />"
		),
		Array(
			"name" => "Checked - Ready to post but awaiting employer",
			"gif" => "<img border='0' src='misc/images/checked_awaiting_employer.gif' alt='' />"
		),
		Array(
			"name" => "Posted - 24 Hour Hold",
			"gif" => "<img border='0' src='misc/images/posted_holding.gif' alt='' />"
		),
		Array(
			"name" => "Posted - Job is student viewable",
			"gif" => "<img border='0' src='misc/images/posted_student_viewable.gif' alt='' />"
		),
		Array(
			"name" => "Closed - Applications not sent",
			"gif" => "<img border='0' src='misc/images/closed_no_applications.gif' alt='' />"
		),
		Array(
			"name" => "Closed - Applications sent",
			"gif" => "<img border='0' src='misc/images/closed_applications_sent.gif' alt='' />"
		),
		Array(
			"name" => "Not Interviewing - No suitable applicants",
			"gif" => "<img border='0' src='misc/images/not_interview_no_suitable.gif' alt='' />"
		),
		Array(
			"name" => "Not Interviewing - No funding",
			"gif" => "<img border='0' src='misc/images/not_interviewing_no_money.gif' alt='' />"
		),
		Array(
			"name" => "Not Interviewing - Other",
			"gif" => "<img border='0' src='misc/images/not_interviewing_other.gif' alt='' />"
		),
		Array(
			"name" => "Interviewing",
			"gif" => "<img border='0' src='misc/images/you_have_an_interview.gif' alt='' />"
		),
		Array(
			"name" => "Waiting for Job Offer",
			"gif" => "<img border='0' src='misc/images/clock.gif' alt='' />"
		),
		Array(
			"name" => "Rankings Received",
			"gif" => "<img border='0' src='misc/images/rankings_received.gif' alt='' />"
		),
		Array(
			"name" => "Filled - By your department",
			"gif" => "<img border='0' src='misc/images/filled_by_department.gif' alt='' />"
		),
		Array(
			"name" => "Filled - By UVic",
			"gif" => "<img border='0' src='misc/images/filled_uvic.gif' alt='' />"
		),
		Array(
			"name" => "Filled - Elsewhere",
			"gif" => "<img border='0' src='misc/images/filled_elsewhere.gif' alt='' />"
		),
		Array(
			"name" => "Unfilled - No suitable applicants",
			"gif" => "<img border='0' src='misc/images/unfilled_no_suitable.gif' alt='' />"
		),
		Array(
			"name" => "Employer not Hiring",
			"gif" => "<img border='0' src='misc/images/employer_not_hiring.gif' alt='' />"
		),
		Array(
			"name" => "Hiring on Hold",
			"gif" => "<img border='0' src='misc/images/hiring_on_hold.gif' alt='' />"
		)
	);

	return $gif_array;
}
// }}}
// {{{ function getAdminStatusNameFromJob($job_id)

/**
 * This function returns the name of the corresponding status for the job id passed in.
 */
function getAdminStatusNameFromJob($job_id)
{
	$sql = ("
		SELECT DISTINCT admin_status
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$admin_status_id = $row["admin_status"];

	switch($admin_status_id)
	{
		case 1:
			$name = ("Not Checked - Employer entered job");
			break;
		case 2:
			$name = ("Not Checked - Staff entered job");
			break;
		case 7:
			$name = ("Checked - Job needs to be coded");
			break;
		case 8:
			$name = ("Checked - Ready to post but awaiting date");
			break;
		case 9:
			$name = ("Checked - Ready to post but awaiting employer");
			break;
		case 11:
			$name = ("Posted - Job is student viewable");
			break;
		case 12:
			$sql = ("
				SELECT DISTINCT job_id
				FROM applications
				WHERE job_id='" . $job_id . "'
				AND app_status IN ('".APP_ACTIVATED_ONLINE."','".APP_PAPER_SENT."')
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			if ($result->RecordCount())
			{
				$name = ("Closed - Applications sent");
			}
			else
			{
				$name = ("Closed - Applications not sent");
			}
			break;
		case 13:
			$name = ("Not Interviewing - No suitable applicants");
			break;
		case 14:
			$name = ("Not Interviewing - No funding");
			break;
		case 15:
			$name = ("Not Interviewing - Other");
			break;
		case 16:
			$name = ("Interviewing");
			break;
		case 17:
			$name = ("Waiting for Job Offer");
			break;
		case 18:
			$name = ("Rankings Received");
			break;
		case 19:
			$filled_dept = getFilledDept($job_id);
			if ($filled_dept == $auth->department)
			{
				$name = ("Filled - By your department");
			}
			else
			{
				$name = ("Filled - By UVic");
			}
			break;
		case 20:
			$name = ("Filled - Elsewhere");
			break;
		case 21:
			$name = ("Unfilled - No suitable applicants");
			break;
		case 22:
			$name = ("Employer not Hiring");
			break;
		case 23:
			$name = ("Hiring on Hold");
			break;
        case 25:
            $name = ("Posted - 24 Hour Hold");
            break;
		default:
			$name = ("&nbsp;");
			break;
	} //switch
	
	return ($name);
}
// }}}
// {{{ function getAdminStatusGif($job_id)

/**
 * This function returns the corresponding gif image for the id number passed in.  Javascript functionality is added 
 * on to allow popup displays of what the icons means.
 */
function getAdminStatusGif($job_id)
{
	$sql = ("
		SELECT DISTINCT admin_status
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$admin_status_id = $row["admin_status"];

	switch($admin_status_id)
	{
		case 1:
			$gif = ("<img border='0' src='misc/images/not_checked_employer.gif' alt='' />");
			break;
		case 2:
			$gif = ("<img border='0' src='misc/images/not_checked_staff.gif' alt='' />");
			break;
		case 7:
			$gif = ("<img border='0' src='misc/images/checked_needs_to_be_coded.gif' alt='' />");
			break;
		case 8:
			$gif = ("<img border='0' src='misc/images/checked_awaiting_date.gif' alt='' />");
			break;
		case 9:
			$gif = ("<img border='0' src='misc/images/checked_awaiting_employer.gif' alt='' />");
			break;
		case 11:
			$gif = ("<img border='0' src='misc/images/posted_student_viewable.gif' alt='' />");
			break;
		case 12:
			$sql = ("
				SELECT DISTINCT job_id
				FROM applications
				WHERE job_id='" . $job_id . "'
				AND app_status IN ('".APP_ACTIVATED_ONLINE."','".APP_PAPER_SENT."')
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			if ($result->RecordCount())
			{
				$gif = ("<img border='0' src='misc/images/closed_applications_sent.gif' alt='' />");
			}
			else
			{
				$gif = ("<img border='0' src='misc/images/closed_no_applications.gif' alt='' />");
			}
			break;
		case 13:
			$gif = ("<img border='0' src='misc/images/not_interview_no_suitable.gif' alt='' />");
			break;
		case 14:
			$gif = ("<img border='0' src='misc/images/not_interviewing_no_money.gif' alt='' />");
			break;
		case 15:
			$gif = ("<img border='0' src='misc/images/not_interviewing_other.gif' alt='' />");
			break;
		case 16:
			$gif = ("<img border='0' src='misc/images/you_have_an_interview.gif' alt='' />");
			break;
		case 17:
			$gif = ("<img border='0' src='misc/images/clock.gif' alt='' />");
			break;
		case 18:
			$gif = ("<img border='0' src='misc/images/rankings_received.gif' alt='' />");
			break;
		case 19:
			$filled_dept = getFilledDept($job_id);
			if ($filled_dept == $auth->department)
			{
				$gif = ("<img border='0' src='misc/images/filled_by_department.gif' alt='' />");
			}
			else
			{
				$gif = ("<img border='0' src='misc/images/filled_uvic.gif' alt='' />");
			}
			break;
		case 20:
			$gif = ("<img border='0' src='misc/images/filled_elsewhere.gif' alt='' />");
			break;
		case 21:
			$gif = ("<img border='0' src='misc/images/unfilled_no_suitable.gif' alt='' />");
			break;
		case 22:
			$gif = ("<img border='0' src='misc/images/employer_not_hiring.gif' alt='' />");
			break;
		case 23:
			$gif = ("<img border='0' src='misc/images/hiring_on_hold.gif' alt='' />");
			break;
		case 25:
			$gif = ("<img border='0' src='misc/images/posted_holding.gif' alt='' />");
			break;
		default:
			$gif = ("&nbsp;");
			break;
	} //switch
	
    if ($gif != "&nbsp;")
    {
        $returnStr = ("<a href='javascript:void(0);' onmouseover=\"return overlib('" . getAdminStatusNameFromJob($job_id) . "');\" onmouseout=\"return nd();\">");
        $returnStr .= $gif . "</a>";
    }
    else
    {
        $returnStr = $gif;
    }

	return ($returnStr);
}
// }}}
// {{{ function getStudentOwnStatusGif($job_id, $student_number)

/** 
 * Function below takes a student number and job id, and returns a string of gifs representing statuses to show 
 * how far that student has progressed in the job.  Javascript functionality is added on to allow popup displays
 * of what the icons means.
 */
function getStudentOwnStatusGif($job_id, $student_number)
{
    $statusGifString = "";

    // Determine how far this student has progressed in this job.
    if (isAdminPulled($job_id, $student_number))
    {
        $statusGifString .= ("<a href='javascript:void(0);' onmouseover=\"return overlib('Your application has been added by admin');\" onmouseout=\"return nd();\">");
        $statusGifString .= ("<img border='0' src='misc/images/your_app_has_been_pulled.gif' alt='' />&nbsp;");
        $statusGifString .= ("</a>");
    }
    elseif (hasStudentApplied($job_id, $student_number))
    {
        $statusGifString .= ("<a href='javascript:void(0);' onmouseover=\"return overlib('You have applied to this job');\" onmouseout=\"return nd();\">");
        $statusGifString .= ("<img border='0' src='misc/images/you_applied.gif' alt='' />&nbsp;");
        $statusGifString .= ("</a>");
    }
    if (isStudentShortlisted($job_id, $student_number) && isShowable($job_id) && hasStudentApplied($job_id, $student_number))
    {
        $statusGifString .= ("<a href='javascript:void(0);' onmouseover=\"return overlib('You have been shortlisted');\" onmouseout=\"return nd();\">");
        $statusGifString .= ("<img border='0' src='misc/images/you_have_been_shortlisted.gif' alt='' />&nbsp;");
        $statusGifString .= ("</a>");
    }
    if (isStudentSignedUp($job_id, $student_number) && isShowable($job_id) && hasStudentApplied($job_id, $student_number))
    {
        $statusGifString .= ("<a href='javascript:void(0);' onmouseover=\"return overlib('You have an interview');\" onmouseout=\"return nd();\">");
        $statusGifString .= ("<img border='0' src='misc/images/you_have_an_interview.gif' alt='' />&nbsp;");
        $statusGifString .= ("</a>");
    }
    if (hasStudentBeenPlacedInJob($job_id, $student_number))
    {
        $statusGifString .= ("<a href='javascript:void(0);' onmouseover=\"return overlib('You have been placed');\" onmouseout=\"return nd();\">");
        $statusGifString .= ("<img border='0' src='misc/images/you_have_been_placed.gif' alt='' />&nbsp;");
        $statusGifString .= ("</a>");
    }
    if (!further_progress($job_id, $student_number) && (isShowable($job_id) || employer_not_hiring($job_id)))
    {
        $statusGifString .= ("<a href='javascript:void(0);' onmouseover=\"return overlib('You did not get this job');\" onmouseout=\"return nd();\">");
        $statusGifString .= ("<img border='0' src='misc/images/no_more_progress.gif' alt='' />&nbsp;");
        $statusGifString .= ("</a>");
    }
    
    // If we have no value for statusGifString yet, set it to a non-breaking space character to avoid <td> problems
    if (!$statusGifString)
    {
        $statusGifString = "&nbsp;";
    }

    return ($statusGifString);
}
// }}}
// {{{ function getStudentStatusGif($job_id)

/**
 * Function below takes a student status, and returns a string of gifs representing the statuses for that job
 * This function also returns the appropriate code to generate a mouse over pop up for that status.
 */
function getStudentStatusGif($job_id)
{
    $sql = ("
        SELECT student_status, resumes_pulled, for_student_comments
        FROM job_info
        WHERE job_id='" . $job_id . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $student_status = $row["student_status"];
    $resumes_pulled = $row["resumes_pulled"];
    $for_student_comments = $row["for_student_comments"];

    switch($student_status)
    {
        case "1":
            $statusGifString = ("&nbsp;<img border='0' src='misc/images/doc10.gif' alt='' />&nbsp;");
        break;
        case "2":
            $statusGifString = ("&nbsp;<img border='0' src='misc/images/cancel.gif' alt='' />&nbsp;");
        break;
        case "3":
            $statusGifString = ("&nbsp;<img border='0' src='misc/images/job_shortlist.gif' alt='' />&nbsp;");
        break;
        case "4":
            $statusGifString = ("&nbsp;<img border='0' src='misc/images/clock.gif' alt='' />&nbsp;");
        break;
        case "5":
            $statusGifString = ("&nbsp;<img border='0' src='misc/images/smchk_gr.gif' alt='' />&nbsp;");
        break;
        case "6":
            $statusGifString = ("&nbsp;<img border='0' src='misc/images/smchk_yl.gif' alt='' />&nbsp;");
        break;
        case "7":
            $statusGifString = ("&nbsp;<img border='0' src='misc/images/X.gif' alt='' />&nbsp;");
        break;
    }

    $student_status = getStudentStatusName($student_status);
    if ($for_student_comments) {
        $student_status .= "<hr />";
        $student_status .= "<u>Notes:</u><br />";
        $student_status .= $for_student_comments;
    }

    $student_status = makeStringOverlibSafe($student_status);

    if ($statusGifString)
    {
        $returnStr = ("<a href='javascript:void(0);' onmouseover=\"return overlib('" . $student_status . "');\" onmouseout=\"return nd();\">");
        $returnStr .= $statusGifString . "</a>";
    }

    if ($resumes_pulled)
    {
        $returnStr .= ("<a href='javascript:void(0);' onmouseover=\"return overlib('" . $student_status . "');\" onmouseout=\"return nd();\">");
        $returnStr .= ("&nbsp;<img border='0' src='misc/images/Warning.gif' alt='' />&nbsp;</a>");
    }

    if (!$returnStr)
    {
        $returnStr = "&nbsp;";
    }
    return ($returnStr);
}
// }}}
// {{{ function getFilledDept($job_id)

/**
 * Function below returns the department that this job was filled from
 */
function getFilledDept($job_id)
{
	// For now this is just a stub.

	return ('1');
}
// }}}
// {{{ function adminStatusString($job_id)

/**
 * Function below returns an array containing the ids of all the admin statuses
 * that can be set currently for the job passed in.
 */
function adminStatusString($job_id)
{
	global $userlevel;

	// Are interviews existant for this job (remember to check for multi-job ints)?
	$sql = ("
		SELECT DISTINCT interview_job_id
		FROM interview_job_join
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if ($result->RecordCount())
	{
		$row = $result->FetchRow();
		$interviewing = true;
		$interview_job_id = $row["interview_job_id"];
	}
	else
	{
		$sql = ("
			SELECT DISTINCT job_id 
			FROM interview
			WHERE job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		if ($result->RecordCount())
		{
			$interviewing = true;
			$interview_job_id = $job_id;
		}
	}

	if ($interviewing)
	{
		// Has the interview finished?

		$sql = ("
			SELECT MAX(int_date) AS end_date, MAX(end_time) AS end_time, NOW()
			FROM interview_time
			WHERE job_id='" . $interview_job_id . "'
			GROUP BY int_date
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();

		$end_datetime = $row["end_date"] . " " . $row["end_time"];
		if ($end_datetime < $row["NOW()"])
		{
			$interview_is_over = true;
		}
	}

	// Pull out the current status and admin_status for this job.
	$sql = ("
		SELECT DISTINCT admin_status, status
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();

	$status_string .= ("'" . $row["admin_status"] . "',");

	if ($interview_is_over)
	{
		// Fourth batch of statuses should be displayed

		$sql = ("
			SELECT DISTINCT status_id
			FROM admin_status
			WHERE status_id>'16'
			AND status_id <> '".HOLDING_ADMIN."'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		while ($row = $result->FetchRow())
		{
			$status_string .= ("'" . $row["status_id"] . "',");
		}
		$status_string = substr($status_string, 0, -1);
	}
	elseif ($interviewing)
	{
		// Third batch of statuses to be displayed

		$sql = ("
			SELECT DISTINCT status_id
			FROM admin_status
			WHERE (status_id>'11' AND status_id<'16')
			OR status_id='19'
            OR status_id='20'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		while ($row = $result->FetchRow())
		{
			$status_string .= ("'" . $row["status_id"] . "',");
		}
		$status_string = substr($status_string, 0, -1);
	}
	elseif ($row["admin_status"] == HOLDING_ADMIN)
	{
		// The job is in holding pattern.  Since only the trafficdirector can set status, return appropriately.
		// They should also be able to cancel the job.

		$sql = ("
			SELECT DISTINCT status_id
			FROM admin_status
			WHERE (status_id>'9' AND status_id<'16')
			OR status_id='19'
            OR status_id='20'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		while ($row = $result->FetchRow())
		{
			$status_string .= ("'" . $row["status_id"] . "',");
		}
		$status_string = substr($status_string, 0, -1);
	}
	elseif ($row["admin_status"] > 16)
	{
		// They got here without an interview
		$sql = ("
			SELECT DISTINCT status_id
			FROM admin_status
			WHERE status_id>'16'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		while ($row = $result->FetchRow())
		{
			$status_string .= ("'" . $row["status_id"] . "',");
		}
		$status_string = substr($status_string, 0, -1);
	}
	elseif ($row["admin_status"] > 10 && $row["admin_status"] < 16)
	{
		// Second batch of statuses to be displayed

		$sql = ("
			SELECT DISTINCT status_id
			FROM admin_status
			WHERE (status_id>'10' AND status_id<'16')
			OR status_id='19'
            OR status_id='20'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		while ($row = $result->FetchRow())
		{
			$status_string .= ("'" . $row["status_id"] . "',");
		}
		$status_string = substr($status_string, 0, -1);
	}
	elseif ($row["admin_status"] < 11)
	{
		// First batch of statuses should be displayed

		$sql = ("
			SELECT DISTINCT status_id
			FROM admin_status
			WHERE (status_id<'13' OR status_id='".HOLDING_ADMIN."')
			");
		if ($userlevel != TRAFFICDIRECTOR)
		{
			$sql .= ("
				AND status_id!='11'
				");
		}
		$result = $GLOBALS['dbh']->Execute($sql);
		while ($row = $result->FetchRow())
		{
			$status_string .= ("'" . $row["status_id"] . "',");
		}
		$status_string = substr($status_string, 0, -1);
	}
	else
	{
		/* 
		 This shouldn't be happening.  Return a blank string so the error is found.
		 Correction, this is happening when a job is taking to the filled at UVic
		 status without being given an interview.
		*/

		$status_string .= ("''");
	}

	return($status_string);
}
// }}}
// {{{ function getHistoryStatus($id)

function getHistoryStatus($id)
{
    $sql = "select history_status_name from history_status";
	$results=$GLOBALS['dbh']->Execute($sql);
	$row=$results->FetchRow(); 
	return $row['history_status_name'];
}
// }}}
// {{{ function getHistoryOptions($id)

function getHistoryOptions($id)
{
	$sql = "select history_options_name from history_options";
	$results=$GLOBALS['dbh']->Execute($sql);
	$row=$results->FetchRow(); 
	return $row['history_options_name'];
}
// }}}
// {{{ function getHistoryReportSubject($id)

function getHistoryReportSubject($id)
{
	$sql = "select report_subject_name from history_report_subject";
	$results=$GLOBALS['dbh']->Execute($sql);
	$row=$results->FetchRow(); 
	return $row['report_subject_name'];
}
// }}}
// {{{ function getSiteVisitType($id)

function getSiteVisitType($id)
{
	$sql = "select description from site_visit_type";
	$results=$GLOBALS['dbh']->Execute($sql);
	$row=$results->FetchRow(); 
	return $row['description'];
}
// }}}
// {{{ function getNextJobCode($term_id, $year)

/**
 * The function below returns the next automated job code for the
 * term and year passed in.
 */
function getNextJobCode($term_id, $year)
{
	// Get the term code

	$sql = ("
		SELECT term_code
		FROM term
		WHERE term_id='" . $term_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$term_code = $row["term_code"];

	// Get the next sequential number for the term and year

	$sql = ("
		SELECT DISTINCT next_sequential_code
		FROM sequential_job_code
		WHERE term_id='" . $term_id . "'
		AND year='" . $year . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if (!$result->RecordCount())
	{
		// First job for this term, so put in the first sequential code
		
		$sql = ("
			INSERT INTO sequential_job_code
			(next_sequential_code, term_id, year)
			VALUES
			(
			'1',
			'" . $term_id . "',
			'" . $year . "'
			)
			");
		$GLOBALS['dbh']->Execute($sql);

		$sequential_code = '1';
	}
	else
	{
		$row = $result->FetchRow();
		$sequential_code = $row["next_sequential_code"];
	}
	$sequential_code = str_pad($sequential_code, 5, '0', STR_PAD_LEFT);
	
	// Increment the sequential code
	
	$sql = ("
		UPDATE sequential_job_code
		SET next_sequential_code=next_sequential_code + 1
		WHERE term_id='" . $term_id . "'
		AND year='" . $year . "'
		");
	$GLOBALS['dbh']->Execute($sql);

	// Create the job code

	$getNextJobCode = $year . $term_code . "-" . $sequential_code;

	return ($getNextJobCode);
}
// }}}
// {{{ function is_app_flagged($app_id, $contact_id)

/**
 * See if the employer has flagged an applicant
 */
function is_app_flagged($app_id, $contact_id)
{
	$sql = ("
		SELECT DISTINCT application_id
		FROM employer_apps_flagged
		WHERE application_id ='" . $app_id . "'
		AND contact_id ='" . $contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if ($result->RecordCount())
	{
		return(true);
	}
	else
	{
		return(false);
	}
}
// }}}
// {{{ function getHoldEndTime()

/**
 * This function is used to determine the date and time that a job being entered
 * now will leave the holding pattern at.  This is set to work for a 24 hour
 * holding pattern, and will need to be modified if someone wishes to use it for a
 * period other than that.
 */
function getHoldEndTime()
{
	$sql = ("
		SELECT NOW()
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$hold_begin_datetime = $row["NOW()"];

	$date_time = split(" ", $hold_begin_datetime);
	$date = split("-", $date_time[0]);
	$time = split(":", $date_time[1]);
	$timestamp = mktime($time[0], $time[1], $time[2], $date[1], $date[2], $date[0]);

	$date_for_week = getdate($timestamp);
	$day_of_week = $date_for_week["wday"];

	if ($day_of_week == 5)
	{
		// If today is Friday, the job closes 72 hours from now.

		$date[2] += 3;

		$end_timestamp = mktime($time[0], $time[1], $time[2], $date[1], $date[2], $date[0]);
		$hold_end_datetime = date("Y-m-d H:i:s", $end_timestamp);
	}
	elseif ($day_of_week == 6 || $day_of_week == 0)
	{
		// Today is Sat or Sun, so we need to set the time to end at 11:59:59 Mon.

		if ($day_of_week == 6)
		{
			$date[2] += 2;
		}
		elseif ($day_of_week == 0)
		{
			$date[1] += 1;
		}

		$mon_timestamp = mktime($time[0], $time[1], $time[2], $date[1], $date[2], $date[0]);
		$mon_date = date("Y-m-d", $mon_timestamp);

		$hold_end_datetime = $mon_date . " 23:59:59";
	}
	else
	{
		$date[2] += 1;
		$end_timestamp = mktime($time[0], $time[1], $time[2], $date[1], $date[2], $date[0]);
		$hold_end_datetime = date("Y-m-d H:i:s", $end_timestamp);
	}
	
	return($hold_end_datetime);
}
// }}}
// {{{ function getHoldTimeRemaining($hold_begin_datetime)

/**
 * This is a pretty specific function.  It returns the amount of time remaining
 * for a job that is in the holding pattern.  This function takes into account
 * jobs that span a weekend.
 */
function getHoldTimeRemaining($hold_begin_datetime)
{
	$date_time = split(" ", $hold_begin_datetime);
	$date = split("-", $date_time[0]);
	$time = split(":", $date_time[1]);
	$timestamp = mktime($time[0], $time[1], $time[2], $date[1], $date[2], $date[0]);
	$date = getdate($timestamp);
	$day_of_week = $date["wday"];

	if ($day_of_week == 5)
	{
		// The job entered the pattern on Friday, so the job closes on Monday.

		$mins_elapsed = cal_length($date_time[1], getCurrentTime());

		// If the current day is Sat, we need to add 24 hours to the min_elapsed.  If it is Sun, 48 hours.

		$current_weekday = date("D");

		if ($current_weekday == "Sat")
		{
			$add_minutes = 24 * 60;
		}
		elseif ($current_weekday == "Sun")
		{
			$add_minutes = 48 * 60;
		}
		elseif ($current_weekday == "Mon")
		{
			$add_minutes = 72 * 60;
		}

		$mins_elapsed += $add_minutes;
		
		$time_remaining = sub_times_2("71:59:59", $mins_elapsed);
	}
	elseif ($day_of_week == 6 || $day_of_week == 0)
	{
		// The job entered the pattern on Saturday or Sunday, so it closes Monday at midnight.

		$mins_remaining = cal_length(getCurrentTime(), "23:59:59");
		$time_remaining = date("H:i:s", mktime(0, $mins_remaining, 0));

		$current_weekday = date("D");
		
		if ($current_weekday == "Sat")
		{
			$split_time = split(":", $time_remaining);
			$split_time[0] += 48;
			$time_remaining = implode(":", $split_time);
		}
		elseif ($current_weekday == "Sun")
		{
			$split_time = split(":", $time_remaining);
			$split_time[1] += 24;
			$time_remaining = implode(":", $split_time);
		}
	}
	else
	{
		$mins_elapsed = cal_length($date_time[1], getCurrentTime());
		$time_remaining = sub_times("23:59:59", $mins_elapsed);
	}

	return($time_remaining);
}
// }}}
// {{{ function getStudentDepartments($student_number, $include_groups = false)

/** 
 * Function returns all of the departments that a student is in.  If specified, this function will also
 * include groups in its search.
 */
function getStudentDepartments($student_number, $include_groups = false)
{
    if (!$student_number)
    {
        return (false);
    }

    $sql = ("
        SELECT DISTINCT d.department_id
        FROM student_department AS sd
        INNER JOIN department AS d
            ON d.department_id=sd.department_id
        INNER JOIN student_flags_join AS sfj
            ON sfj.record_id=sd.record_id
        WHERE sd.student_number='" . $student_number . "'
        AND sfj.student_flags_id = '". CURRENT_FLAG ."'
        ORDER BY d.department_name
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    while ($row = $result->FetchRow())
    {
        $dept_ids[] = $row["department_id"];
    }

    if ($include_groups)
    {
        // If they want to include grouped department's in the function, do so.
        foreach ($dept_ids AS $di)
        {
            $depts_in_group = department_groups($di);
            foreach ($depts_in_group AS $dig)
            {
                if (!is_array($all_depts) || !in_array($dig, $all_depts))
                {
                    $all_depts[] = $dig;
                }
            }
        }
        // We've now got an array with all of the departments in it, including groups.  Set $dept_ids to point at this array.
        $dept_ids = $all_depts;
    }

    // Now we're ready to go.  Return dept_ids.
    return ($dept_ids);
}
// }}}
// {{{ function getDisciplinesForDepts($dept_ids)

/**
 * Function returns all of the disciplines that a given set of department_ids have access to.  $dept_ids can either be a string
 * holding one department_id, or an array holding many.
 */
function getDisciplinesForDepts($dept_ids)
{
    // Is it one or many dept ids we have?
    if (!$dept_ids)
    {
        return (false);
    }
    elseif (is_array($dept_ids))
    {
        foreach ($dept_ids AS $di)
        {
            $sql = ("
                SELECT DISTINCT ddj.department_id, dd.discipline_id, dd.discipline_code, dd.discipline_name
                FROM department_disciplines AS dd
                INNER JOIN department_discipline_join AS ddj
                ON dd.discipline_id=ddj.discipline_id
                WHERE ddj.department_id='" . $di . "'
                ORDER BY dd.discipline_name
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row = $result->FetchRow())
            {
                if (!is_array($discipline_ids) || !in_array($row["discipline_id"], $discipline_ids))
                {
                    $discipline_ids[] = $row["discipline_id"];
                    $discipline_array[$di][] = Array(
                        "id" => $row["discipline_id"],
                        "code" => $row["discipline_code"],
                        "name" => $row["discipline_name"],
                        "dept_id" => $row["department_id"]
                        );
                }
            }
            unset($discipline_ids);
        }
    }
    elseif (is_string($dept_ids))
    {
        $sql = ("
            SELECT DISTINCT ddj.department_id, dd.discipline_id, dd.discipline_code, dd.discipline_name
            FROM department_disciplines AS dd
            INNER JOIN department_discipline_join AS ddj
            ON dd.discipline_id=ddj.discipline_id
            WHERE ddj.department_id='" . $dept_ids . "'
            ORDER BY dd.discipline_name
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        while ($row = $result->FetchRow())
        {
            if (!is_array($discipline_ids) || !in_array($row["discipline_id"], $discipline_ids))
            {
                $discipline_ids[] = $row["discipline_id"];
                $discipline_array[$dept_ids][] = Array(
                    "id" => $row["discipline_id"],
                    "code" => $row["discipline_code"],
                    "name" => $row["discipline_name"],
                    "dept_id" => $row["department_id"]
                    );
            }
        }
    }

    // We should now have the discipline_array.  Let's return it.

    return ($discipline_array);
}
// }}}
// {{{ function getJobsWithContact($contact_id, $includeClosed = false)

/**
 * Function below returns current or future jobs associated with the contact passed in.
 */
function getJobsWithContact($contact_id, $includeClosed = false)
{
    $current_term_id = getCurrentTermID();
    $current_year = getCurrentYear();

    if (!$includeClosed)
    {
        // Only check for current and future jobs.
        $sql = ("
            SELECT DISTINCT ji.job_id
            FROM job_info AS ji
            INNER JOIN term AS t ON t.term_id=ji.term_id
            INNER JOIN term AS t2 ON t2.term_id='" . $current_term_id . "'
            WHERE ji.contact_id='" . $contact_id . "'
            AND (ji.year>'" . $current_year . "'
            OR (ji.year='" . $current_year . "' AND t.year_order>=t2.year_order)
            AND ji.status NOT IN ('" . INT_ONLY . "', '". CANCELLED . "', '" . REJECTED . "')
            )
            ");
    }
    else
    {
        // Check for current, future, and past jobs.
        $sql = ("
            SELECT DISTINCT ji.job_id
            FROM job_info AS ji
            WHERE ji.contact_id='" . $contact_id . "'
            AND ji.status NOT IN ('" . INT_ONLY . "')
            ");
    }
    $result = $GLOBALS['dbh']->Execute($sql);
    if ($result->RecordCount())
    {
        while ($row = $result->FetchRow())
        {
            $jobsWithContact[] = $row["job_id"];
        }
        return $jobsWithContact;
    }
    else
    {
        return false;
    }
}
// }}}
// {{{ function getJobsWithDivision($division_id)

/**
 * Function below returns current or future jobs associated with the division passed in.
 */
function getJobsWithDivision($division_id)
{
    $current_term_id = getCurrentTermID();
    $current_year = getCurrentYear();

    $sql = ("
        SELECT DISTINCT ji.job_id
        FROM job_info AS ji
        INNER JOIN term AS t ON t.term_id=ji.term_id
        INNER JOIN term AS t2 ON t2.term_id='" . $current_term_id . "'
        WHERE ji.employer_department_id='" . $division_id . "'
        AND (ji.year>'" . $current_year . "'
        OR (ji.year='" . $current_year . "' AND t.year_order>=t2.year_order)
        AND ji.status NOT IN ('" . INT_ONLY . "', '". CANCELLED . "', '" . REJECTED . "')
        )
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    if ($result->RecordCount())
    {
        while ($row = $result->FetchRow())
        {
            $jobsWithDivision[] = $row["job_id"];
        }
        return $jobsWithDivision;
    }
    else
    {
        return false;
    }
}
// }}}
// {{{ function getSpecialTermYear ()

function getSpecialTermYear ()
{
    // let's figure out which term they'd be filling this out for: half way between either term
    /* assumptions being made:
        - terms don't span over the new year: end_date >  start_date (ie: NOT start nov. & end feb.)
        - terms are consecutive with no breaks in between, therefore:
          12 (months) divided by the number of term_ids (instances in the term table), should be the terms
    */

    $result = $GLOBALS['dbh']->Execute("SELECT DATE_FORMAT(CURRENT_DATE(), '%Y %m %d') as current");
    $row = $result->FetchRow();
    list($year, $month, $day) = explode(" ", $row["current"]);
    $day = 1;
    switch($month){
        case 1:
        case 2:
            $year = $year -1;
        case 11:
        case 12:
            $month = 11;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            $month = 3;
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            $month = 7;
            break;
    }
    /*
    $sql = "SELECT term_id, start_date, end_date
        FROM term";
    $result = $GLOBALS['dbh']->Execute ($sql);

    // this seems to do what's needed ... so let's just keep it
    $this_month = date('m', time()) + (ceil ((12 / $result->RecordCount())) - 1);
    if ($this_month > 12)
    {
        $eval_year = date('Y', time()) + 1;
        $this_month = $this_month - 12;
    }
    else
    {
        $eval_year = date('Y', time());
    }

    while (($row = $result->FetchRow()) && (!$eval_term_id))
    {
        // sneaky trick that (hopefully) works, by adding months onto $this_month (as above)
        // this should either bump it into the next term semester or keep it in the current proper one
        if (($this_month >= $row["start_date"]) && ($this_month <= $row["end_date"]))
        {
            $eval_term_id = $row["term_id"];
        }
    }
    */
    $sql = "SELECT term_id FROM term WHERE '".$month."' >= start_date AND '".$month."' <= end_date";
    $result = $GLOBALS['dbh']->Execute ($sql);
    $row = $result->FetchRow();

    $eval_term_id = $row['term_id'];
    $eval_year = $year;

    $specialTermYear = array(
            "eval_term_id" => $eval_term_id,
            "eval_year" => $eval_year);

    return $specialTermYear;
}
// }}}
// {{{ function updateDivisionStatus($division_id, $department_id)

/*
 The function below is called whenever a contact has its status updated.  This function takes the status that has been saved for the
 contact, and checks to see if the division needs to have their status updated as well.
*/

function updateDivisionStatus($division_id, $department_id)
{
    // First pull out the highest level of activity we have for a contact under this division.
    $sql = ("
        SELECT DISTINCT eis.level_of_activity, eis.status_id, ce.contact_id
        FROM employer_info_statuses AS eis
        INNER JOIN contact_employer AS ce
        ON ce.department_id='" . addslashes(trim($division_id)) . "'
        INNER JOIN department_contact_status AS dcs
        ON dcs.contact_id=ce.contact_id
        AND dcs.department_id='" . addslashes(trim($department_id)) . "'
        AND eis.status_id=dcs.status_id
        WHERE dcs.current_status = '1'
        ORDER BY eis.level_of_activity ASC, dcs.activity_date DESC, dcs.entered_on DESC
        LIMIT 1
        ");
    $result = $GLOBALS['dbh']->Execute($sql);

    // Now we have a set of results that contain the level's of activity for all of the statuses set for all the contact's at this division.
    // Next step is to grab the highest level of activity.  This level of activity must correspond to a status that is most recent, for one of
    // these contacts.

    $row = $result->FetchRow();
    $max_LOA = $row['level_of_activity'];
    $max_status_id = $row['status_id'];

    // $max_LOA and $max_status_id should now be set with the appropriate values.  Now we just update the division with this status, if needed.
    $sql = ("
        SELECT DISTINCT eis.level_of_activity, eis.status_id
        FROM employer_info_statuses AS eis
        INNER JOIN department_division_status AS dds
        ON dds.division_id='" . addslashes(trim($division_id)) . "'
        AND dds.department_id='" . addslashes(trim($department_id)) . "'
        AND eis.status_id=dds.status_id
        WHERE dds.current_status = '1'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    if ($result->RecordCount())
    {
        if ($row["level_of_activity"] != $max_LOA)
        {
            // The division needs to have its status updated.
            $sql = ("
                INSERT INTO department_division_status
                (department_id, division_id, status_id, entered_on)
                VALUES (
                    '" . addslashes(trim($department_id)) . "',
                    '" . addslashes(trim($division_id)) . "',
                    '" . addslashes(trim($max_status_id)) . "',
                    now()
                )
                ");
            $GLOBALS['dbh']->Execute($sql);
            updateDivisionCurrentStatus($division_id,$department_id);
        }
    }
    // The division has no statuses at all, start it off as potential lead then. 
    elseif (!$result->RecordCount() && $max_status_id)
    {
        $sql = ("
            INSERT INTO department_division_status
            (department_id, division_id, status_id, entered_on)
            VALUES (
                '" . addslashes(trim($department_id)) . "',
                '" . addslashes(trim($division_id)) . "',
                '" . addslashes(trim($max_status_id)) . "',
                now()
            )
            ");
        $GLOBALS['dbh']->Execute($sql);
        updateDivisionCurrentStatus($division_id,$department_id);
    }
    else
    {
        $sql = ("
            INSERT INTO department_division_status
            (department_id, division_id, status_id, entered_on)
            VALUES (
                '" . addslashes(trim($department_id)) . "',
                '" . addslashes(trim($division_id)) . "',
                '" . CONTACT_POTENTIAL_LEAD . "',
                now()
            )
            ");
        $GLOBALS['dbh']->Execute($sql);
        updateDivisionCurrentStatus($division_id,$department_id);
    }
}
// }}}
// {{{ function updateCompanyStatus($employer_id, $department_id)

/*
 The function below is called whenever a contact has its status updated.  This function takes the status that has been saved for the
 contact, and checks to see if the company needs to have their status updated as well.
*/

function updateCompanyStatus($employer_id, $department_id)
{
    // First pull out the highest level of activity we have for a division under this company.
    $sql = ("
        SELECT DISTINCT eis.level_of_activity, eis.status_id
        FROM employer_info_statuses AS eis
        INNER JOIN employer_department AS ed
        ON ed.employer_id = '". addslashes(trim($employer_id)) . "'
        INNER JOIN department_division_status AS dds
        ON ed.department_id = dds.division_id
        AND dds.department_id = '".addslashes(trim($department_id))."' 
        AND dds.status_id = eis.status_id
        WHERE dds.current_status = '1'
        ORDER BY eis.level_of_activity ASC
        LIMIT 1
        ");
    $result = $GLOBALS['dbh']->Execute($sql);

    // Now we have a set of results that contain the level's of activity for all of the statuses set for all the contact's at this company.
    // Next step is to grab the highest level of activity.  This level of activity must correspond to a status that is most recent, for one of
    // these contacts.
    $row = $result->FetchRow();
    $max_LOA = $row["level_of_activity"];
    $max_status_id = $row["status_id"];

    // $max_LOA and $max_status_id should now be set with the appropriate values.  Now we just update the company with this status, if needed.
    $sql = ("
        SELECT DISTINCT eis.level_of_activity, eis.status_id
        FROM employer_info_statuses AS eis
        INNER JOIN department_company_status AS dcos
        ON dcos.employer_id='" . addslashes(trim($employer_id)) . "'
        AND dcos.department_id='" . addslashes(trim($department_id)) . "'
        AND eis.status_id=dcos.status_id
        WHERE dcos.current_status = '1'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    if ($result->RecordCount())
    {
        if ($row["level_of_activity"] != $max_LOA && $max_status_id)
        {
            // The company needs to have its status updated.
            $sql = ("
                INSERT INTO department_company_status
                (department_id, employer_id, status_id, entered_on)
                VALUES (
                    '" . addslashes(trim($department_id)) . "',
                    '" . addslashes(trim($employer_id)) . "',
                    '" . addslashes(trim($max_status_id)) . "',
                    now()
                )
                ");
            $GLOBALS['dbh']->Execute($sql);

            updateCompanyCurrentStatus($employer_id,$department_id);
        }
    }
    elseif (!$result->RecordCount() && $max_status_id)
    {
        $sql = ("
            INSERT INTO department_company_status
            (department_id, employer_id, status_id, entered_on)
            VALUES (
                '" . addslashes(trim($department_id)) . "',
                '" . addslashes(trim($employer_id)) . "',
                '" . addslashes(trim($max_status_id)) . "',
                now()
            )
            ");
        $GLOBALS['dbh']->Execute($sql);
        updateCompanyCurrentStatus($employer_id,$department_id);
    }
    // Company has no status, start it off with potential lead. 
    else
    {
        $sql = ("
            INSERT INTO department_company_status
            (department_id, employer_id, status_id, entered_on)
            VALUES (
                '" . addslashes(trim($department_id)) . "',
                '" . addslashes(trim($employer_id)) . "',
                '" . CONTACT_POTENTIAL_LEAD . "',
                now()
            )
            ");
        $GLOBALS['dbh']->Execute($sql);
        updateCompanyCurrentStatus($employer_id,$department_id);
    }
}
// }}}
// {{{ function getNextContactStatus($contact_id, $department_id, $term_id, $year) 

function getNextContactStatus($contact_id, $department_id, $term_id, $year) 
{
    $arrResult = array();

    // convert term_id, year into time stamp. See if term/year is in past, if so give them inactive status. 
    $adjusted_date = getTempDateEntered($term_id, $year);

    // If this status is for the past
    if (strtotime($adjusted_date) < strtotime(date("Y-m-d")))
    {
        // Check to see if there are any statuses ahead of this adjusted date
        $sql = ("
            SELECT count(*) AS num_statuses
            FROM department_contact_status
            WHERE activity_date >= '".$adjusted_date."' AND contact_id = '".$contact_id."' AND department_id = '".$department_id."'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        // If there aren't any, then we can use the normal method of determining the next contact status. We use the switch statement below. 
        if ($row['num_statuses'] == 0)
        {
            // get next status based on current status
            $sql = ("
                SELECT status_id
                FROM department_contact_status
                WHERE current_status = '1' AND contact_id = '".$contact_id."' AND department_id = '".$department_id."'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $status = $row['status_id'];
        }
        // Otherwise, there are statuses that are ahead of the adjusted date, we will have to get a status and re-evalate the contact's status again. 
        else
        {
            // get status that's closest to the adjusted date
            $sql = ("
                SELECT status_id
                FROM department_contact_status
                WHERE activity_date <= '".$adjusted_date."' AND contact_id = '".$contact_id."' AND department_id = '".$department_id."'
                ORDER BY activity_date DESC, entered_on DESC
                LIMIT 1
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            if ($result->RecordCount() == 0)
            {
                $status = CONTACT_POTENTIAL_LEAD;
            }
            else
            {
                $row = $result->FetchRow();
                $status = $row['status_id'];
            }

            $arrResult['reevaluate'] = 1;
        }
    }
    // Otherwise, get the next status as usual. 
    else
    {
        $sql = ("
            SELECT status_id
            FROM department_contact_status
            WHERE current_status = '1' AND contact_id = '".$contact_id."' AND department_id = '".$department_id."'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        if ($result->RecordCount() > 1)
        {
            // report something's wrong because there should only be one current_status at any given time. 
            return -1;
        }
        elseif ($result->RecordCount() == 0)
        {
            $next_status = CONTACT_POTENTIAL_LEAD;
        }
        else
        {
            $row = $result->FetchRow();
            $status = $row['status_id'];
        }
    }
    
    if (!$next_status && $status)
    {
        switch($status)
        {
            case CONTACT_POTENTIAL_LEAD:
            case CONTACT_ACTIVE_NEW:
                $next_status = CONTACT_ACTIVE_NEW;
                break;
            case CONTACT_INACTIVE_VETERAN:
            case CONTACT_ACTIVE_VETERAN:
                $next_status = CONTACT_ACTIVE_VETERAN;
                break;
            case CONTACT_INACTIVE_LAPSED:
            case CONTACT_ACTIVE_LAPSED:
                $next_status = CONTACT_ACTIVE_LAPSED;
                break;
            default:
                $next_status = CONTACT_POTENTIAL_LEAD;
        }
    }

    $arrResult['status'] = $next_status;
    return $arrResult;
}
// }}}
// {{{ function reevaluateContactStatus($contact_id, $department_id)

/**
 * Function: reevaluateContactStatus
 * This function is used to reevaluate a given contact's status, using the rules provided in initial_contact_status_conversion.pl.  This function
 * is usually called after two contacts have been merged, and the status is set for the given $department_id.
 * 
 * This function returns an array with the following keys and values:
 * 
 * status => This is the status_id that the contact should now be set with.
 * activity_date => This is the date that the new status should be saved with.
 */
function reevaluateContactStatus($contact_id, $department_id)
{
    //:TODO: For now, we are assuming the existence of a date_entered field in history, and a temp_date_entered field in job_info that
    // hold the relative dates that these jobs and history should be regarded as having for purposes of deteremining statuses.

    // Now we need to calculate the term_id and year that corresponds to TWO terms ahead of the current one.  This is used to pull out
    // activities corresponding to the jobs in the future.
    $sql = ("
        SELECT DISTINCT year_order
        FROM term
        WHERE start_date<=\"" . date("m") . "\"
        AND end_date>=\"" . date("m") . "\"
        ");
    $term_result = $GLOBALS['dbh']->Execute($sql);
    $term_row = $term_result->FetchRow();

    $two_terms_ahead_term = $term_row["year_order"];
    $two_terms_ahead_term += 2;
    $two_terms_ahead_year = date("Y");
    if ($two_terms_ahead_term > 3)
    {
        // Increment the year, and normalize $two_terms_ahead_term again.
        $two_terms_ahead_term -= 3;
        $two_terms_ahead_year++;
    }

    // Now get the starting date of the term after this one.  This is used to pull out activities corresponding to placements in the future.

    // The functionality below may seem redundant, since we could just draw out the start_date of the term we are in, and increment that by 4,
    // but it is better not to make the assumption that a term will always be 4 months long.
    $one_term_ahead_year = date("Y");
    $year_order = $term_row["year_order"];
    // Advance the year order by 1 terms to get the term after next, then increment the year if necessary.
    $year_order++;
    if ($year_order > 3)
    {
        $year_order -= 3;
        $one_term_ahead_year++;
    }

    $sql = ("
        SELECT DISTINCT start_date
        FROM term
        WHERE year_order=\"" . $year_order . "\"
        ");
    $result4 = $GLOBALS['dbh']->Execute($sql);
    $row4 = $result4->FetchRow();
    $one_term_ahead_month = $row4["start_date"];

    // Now create the future date that we are using to search.
    $one_term_ahead_date = date("Y-m-d", mktime(0, 0, 0, $one_term_ahead_month, 1, $one_term_ahead_year));

    // These queries will let us save some time in the while loop.
    $sql = ("
        SELECT contact_id
        FROM job_info
        WHERE contact_id='" . addslashes(trim($contact_id)) . "'
        AND status IN ('".POSTED."','".CLOSED."','".CANCELLED."')
        ");
    $job_info_result = $GLOBALS['dbh']->Execute($sql);

    $sql = ("
        SELECT DISTINCT temp_job_contact
        FROM history
        WHERE temp_job_contact='" . addslashes(trim($contact_id)) . "'
        ");
    $history_job_contact_result = $GLOBALS['dbh']->Execute($sql);

    $sql = ("
        SELECT DISTINCT temp_supervisor_contact
        FROM history
        WHERE temp_supervisor_contact = '" . addslashes(trim($contact_id)) . "'
        ");
    $history_supervisor_contact_result = $GLOBALS['dbh']->Execute($sql);

    if (!$job_info_result->RecordCount() && !$history_job_contact_result->RecordCount() && !$history_supervisor_contact_result->RecordCount())
    {
        $status = CONTACT_POTENTIAL_LEAD;
    }
    else
    {
        // Draw out any job_ids of jobs that have been entered in the future.  If we have any matching this criteria,
        // we need to treat them differently (since the activity will be set in the future).
        $sql = ("
            SELECT DISTINCT ji.job_id, ji.term_id, ji.year
            FROM job_info AS ji
            INNER JOIN department_job_join AS djj
            ON djj.job_id=ji.job_id
            INNER JOIN term AS t
            ON t.term_id=ji.term_id
            AND djj.department_id=\"" . $department_id . "\"
            WHERE ji.contact_id=\"" . $contact_id . "\"
            AND ji.status IN ('".POSTED."','".CLOSED."','".CANCELLED."')
            AND (year>\"" . $two_terms_ahead_year . "\" OR (year=\"" . $two_terms_ahead_year . "\" AND t.year_order>=\"" . $two_terms_ahead_term . "\"))
            ORDER BY ji.year, t.year_order DESC
            ");
        $result2 = $GLOBALS['dbh']->Execute($sql);

        // Draw out any history_ids of placements that would have been entered for the future.
        $sql = ("
            SELECT DISTINCT h.history_id, h.temp_date_entered
            FROM history AS h
            WHERE h.department_id=\"" . $department_id . "\"
            AND (temp_job_contact=\"" . $contact_id . "\" OR temp_supervisor_contact=\"" . $contact_id . "\")
            AND h.temp_date_entered>=\"" . $one_term_ahead_date . "\"
            ORDER BY h.temp_date_entered DESC
            ");
        $result3 = $GLOBALS['dbh']->Execute($sql);
        
        if ($result2->RecordCount() || $result3->RecordCount())
        {
            // We have jobs/placements in the future, so we need to save their date of activity.
            // First assign $activity_date to that of the job (if we have one), and then, if we have a history activity date and it
            // is greater than the job's activity date, replace $activity_date with that.
            if ($row2 = $result2->FetchRow())
            {
                // Now we have to find the start_date of the term BEFORE this job's term and year.
                $sql = ("
                    SELECT DISTINCT year_order
                    FROM term
                    WHERE term_id=\"" . $row2["term_id"] . "\"
                    ");
                $term_result = $GLOBALS['dbh']->Execute($sql);
                $term_row = $term_result->FetchRow();
                $activity_term = $term_row["year_order"];
                $activity_year = $row2["year"];
                $activity_term--;
                if ($activity_term < 1)
                {
                    //:TODO: Term shouldn't be hardcoded, should count how many terms we have in the term table. 
                    // We've wound back into a previous year, so normalize the term, and decrement the year.
                    $activity_term += 3;
                    $activity_year--;
                }
                // Now get the corresponding start_date for this term.
                $sql = ("
                    SELECT DISTINCT start_date
                    FROM term
                    WHERE year_order=\"" . $activity_term . "\"
                    ");
                $activity_month_result = $GLOBALS['dbh']->Execute($sql);
                $activity_month_row = $activity_month_result->FetchRow();
                $activity_month = $activity_month_row["start_date"];

                $activity_date = date("Y-m-d", mktime(0, 0, 0, $activity_month, 1, $activity_year));
            }

            $row3 = $result3->FetchRow();
            if ($row3["temp_date_entered"] > $activity_date)
            {
                $activity_date = $row3["temp_date_entered"];
            }
        }
        else
        {
            // Draw out any job_ids of jobs that would have been entered within the past term, or the future.
            $sql = ("
                SELECT DISTINCT ji.job_id
                FROM job_info AS ji
                INNER JOIN department_job_join AS djj
                ON djj.job_id=ji.job_id
                AND djj.department_id=\"" . $department_id . "\"
                WHERE ji.contact_id=\"" . $contact_id . "\"
                AND ji.status IN ('".POSTED."','".CLOSED."','".CANCELLED."')
                AND ji.temp_date_entered>=\"" . (date("Y-m-d", mktime(0, 0, 0, (date("m") - GRACE_PERIOD)))) . "\"
                ");
            $result2 = $GLOBALS['dbh']->Execute($sql);

            // Draw out any history_ids of placements that would have been entered within the past term, or the future.
            $sql = ("
                SELECT DISTINCT h.history_id
                FROM history AS h
                WHERE h.department_id=\"" . $department_id . "\"
                AND (temp_job_contact=\"" . $contact_id . "\" OR temp_supervisor_contact=\"" . $contact_id . "\")
                AND h.temp_date_entered>=\"" . (date("Y-m-d", mktime(0, 0, 0, (date("m") - GRACE_PERIOD)))) . "\"
                ");
            $result3 = $GLOBALS['dbh']->Execute($sql);
        }

        if (!$result2->RecordCount() && !$result3->RecordCount())
        {
            // No jobs/placements this term for this contact - do they have jobs in the previous three terms?
            $sql = ("
                SELECT DISTINCT ji.job_id
                FROM job_info AS ji
                INNER JOIN department_job_join AS djj
                ON djj.job_id=ji.job_id
                AND djj.department_id=\"" . $department_id . "\"
                WHERE ji.contact_id=\"" . $contact_id . "\"
                AND ji.status IN ('".POSTED."','".CLOSED."','".CANCELLED."')
                AND ji.temp_date_entered>=\"" . (date("Y-m-d", mktime(0, 0, 0, (date("m") - GRACE_PERIOD * 4)))) . "\"
                AND ji.temp_date_entered<\"" . (date("Y-m-d", mktime(0, 0, 0, (date("m") - GRACE_PERIOD)))) . "\"
                ");
            $result2 = $GLOBALS['dbh']->Execute($sql);

            $sql = ("
                SELECT DISTINCT h.history_id
                FROM history AS h
                WHERE h.department_id=\"" . $department_id . "\"
                AND (temp_job_contact=\"" . $contact_id . "\" OR temp_supervisor_contact=\"" . $contact_id . "\")
                AND h.temp_date_entered>=\"" . (date("Y-m-d", mktime(0, 0, 0, (date("m") - GRACE_PERIOD * 4)))) . "\"
                AND h.temp_date_entered<\"" . (date("Y-m-d", mktime(0, 0, 0, (date("m") - GRACE_PERIOD)))) . "\"
                ");
            $result3 = $GLOBALS['dbh']->Execute($sql);

            if ($result2->RecordCount() || $result3->RecordCount())
            {
                // No jobs/placements this term, but they HAVE had jobs/placements in the previous three terms, so -> Inactive Veteran
                $status = CONTACT_INACTIVE_VETERAN;
            }
            else
            {
                // No jobs/placements in the previous three terms, nor the current term.  Do they have ANY jobs/placements?
                $sql = ("
                    SELECT DISTINCT ji.job_id
                    FROM job_info AS ji
                    INNER JOIN department_job_join AS djj
                    ON djj.job_id=ji.job_id
                    AND djj.department_id=\"" . $department_id . "\"
                    WHERE ji.contact_id=\"" . $contact_id . "\"
                    AND ji.status IN ('".POSTED."','".CLOSED."','".CANCELLED."')
                    ");
                $result2 = $GLOBALS['dbh']->Execute($sql);

                $sql = ("
                    SELECT DISTINCT h.history_id
                    FROM history AS h
                    WHERE h.department_id=\"" . $department_id . "\"
                    AND (temp_job_contact=\"" . $contact_id . "\" OR temp_supervisor_contact=\"" . $contact_id . "\")
                    ");
                $result3 = $GLOBALS['dbh']->Execute($sql);

                if ($result2->RecordCount() || $result3->RecordCount())
                {
                    // They have had jobs/placements at some point in the past, so -> Inactive Lapsed
                    $status = CONTACT_INACTIVE_LAPSED;
                }
                else
                {
                    // They've never had a job/placement, so -> Potential Lead
                    $status = CONTACT_POTENTIAL_LEAD;
                }
            }
        }
        else
        {
            // They HAVE got a job/placement this term.  Have they posted jobs/placements in any of the previous three terms?
            $sql = ("
                SELECT DISTINCT ji.job_id
                FROM job_info AS ji
                INNER JOIN department_job_join AS djj
                ON djj.job_id=ji.job_id
                AND djj.department_id=\"" . $department_id . "\"
                WHERE ji.contact_id=\"" . $contact_id . "\"
                AND ji.status IN ('".POSTED."','".CLOSED."','".CANCELLED."')
                AND ji.temp_date_entered>=\"" . (date("Y-m-d", mktime(0, 0, 0, (date("m") - GRACE_PERIOD * 4)))) . "\"
                AND ji.temp_date_entered<\"" . (date("Y-m-d", mktime(0, 0, 0, (date("m") - GRACE_PERIOD)))) . "\"
                ");
            $result2 = $GLOBALS['dbh']->Execute($sql);

            $sql = ("
                SELECT DISTINCT h.history_id
                FROM history AS h
                WHERE h.department_id=\"" . $department_id . "\"
                AND (temp_job_contact=\"" . $contact_id . "\" OR temp_supervisor_contact=\"" . $contact_id . "\")
                AND h.temp_date_entered>=\"" . (date("Y-m-d", mktime(0, 0, 0, (date("m") - GRACE_PERIOD * 4)))) . "\"
                AND h.temp_date_entered<\"" . (date("Y-m-d", mktime(0, 0, 0, (date("m") - GRACE_PERIOD)))) . "\"
                ");
            $result3 = $GLOBALS['dbh']->Execute($sql);

            if ($result2->RecordCount() || $result3->RecordCount())
            {
                // They have also posted/hired in the previous three terms, so -> Active Veteran
                $status = CONTACT_ACTIVE_VETERAN;
            }
            else
            {
                // They have not posted/hired in the previous three terms.  Have they ever posted/hired before this term?
                $sql = ("
                    SELECT DISTINCT ji.job_id
                    FROM job_info AS ji
                    INNER JOIN department_job_join AS djj
                    ON djj.job_id=ji.job_id
                    AND djj.department_id=\"" . $department_id . "\"
                    WHERE ji.contact_id=\"" . $contact_id . "\"
                    AND ji.status IN ('".POSTED."','".CLOSED."','".CANCELLED."')
                    AND (ji.temp_date_entered<\"" . (date("Y-m-d", mktime(0, 0, 0, (date("m") - GRACE_PERIOD * 4)))) . "\" OR ji.temp_date_entered IS NULL)
                    ");
                $result2 = $GLOBALS['dbh']->Execute($sql);

                $sql = ("
                    SELECT DISTINCT h.history_id
                    FROM history AS h
                    WHERE h.department_id=\"" . $department_id . "\"
                    AND (temp_job_contact=\"" . $contact_id . "\" OR temp_supervisor_contact=\"" . $contact_id . "\")
                    AND (h.temp_date_entered<\"" . (date("Y-m-d", mktime(0, 0, 0, (date("m") - GRACE_PERIOD * 4)))) . "\" OR h.temp_date_entered IS NULL)
                    ");
                $result3 = $GLOBALS['dbh']->Execute($sql);
                
                if ($result2->RecordCount() || $result3->RecordCount())
                {
                    // They have posted/hired before, just not in the previous three terms, so -> Active Lapsed
                    $status = CONTACT_ACTIVE_LAPSED;
                }
                else
                {
                    // They've only posted/hired in the current term, so -> Active New
                    $status = CONTACT_ACTIVE_NEW;
                }
            }
        }
    }
        
    if (!$activity_date)
    {
        $activity_date = date("Y-m-d H:i:s");
    }

    $return_array = Array("status" => $status, "activity_date" => $activity_date);

    return $return_array;
}
// }}}
// {{{ function getSupervisorContactID($history_id)

/**
 * The function below takes in a history id, and then from that, attempts to gather the contact_id of the supervisor for that history record.
 * This function will not always be able to find a matching contact, in that case, it will return 0 (false);
 */
function getSupervisorContactID($history_id)
{
    // First things first.  Pull out the relevant information from history.
    $sql = ("
        SELECT supervisor_name, employer_id, company_department_id, supervisor_email, supervisor_phone_number
        FROM history
        WHERE history_id='" . $history_id . "'
        ");
    $diff_result = $GLOBALS['dbh']->Execute($sql);
    $diff_row = $diff_result->FetchRow();

    $fix_sup_name = trim($diff_row['supervisor_name']);
    
    //gets rid of credentials (ie. Dr. Geoffrey Voss, Ph.D)
    // and visits with two supervisors (ie. John Nafsika / Pam Lum <- will choose John Nafsika)
    $fix_sup_name = preg_replace("/\/.*/","", $fix_sup_name);
    $fix_sup_name = preg_replace("/,.*/","", $fix_sup_name);
   
    // gets ride of Dr, Ms, etc 
    $fix_sup_name = preg_replace("/^Dr\.*/", "", $fix_sup_name);
    $fix_sup_name = preg_replace("/^Mrs\.*/", "", $fix_sup_name);
    $fix_sup_name = preg_replace("/^Mr\.*/", "", $fix_sup_name);
    $fix_sup_name = preg_replace("/^Ms\.*/", "", $fix_sup_name);
   
    // split first and last name 
    $fix_sup_name =trim($fix_sup_name);
    $arr_sup_name = explode(" ", $fix_sup_name);
    $sup_first_name = trim($arr_sup_name[0]);
   
    // gets rid of titles such asi, 'P.B.' 
    if(preg_match("/\./", $arr_sup_name[0], $matches) && $arr_sup_name[2])
    {
        $sup_first_name = trim($arr_sup_name[1]);
        $sup_last_name = trim($arr_sup_name[2]);
    }
    
    //Gets rid of middle names (if there are at least three in this array (Jon P Smith), rearrange (Jon Smith))
    if($arr_sup_name[2])
    {
        $sup_last_name = trim($arr_sup_name[2]);
    }
    else
    {
        $sup_last_name = trim($arr_sup_name[1]);
    }
    
    // Now grab the appropriate supervisor information
    $sql = ("
        SELECT c.contact_id
        FROM contact AS c
        INNER JOIN contact_employer AS ce
        ON ce.contact_id=c.contact_id
        WHERE c.first_name='" . addslashes($sup_first_name) . "'
        AND c.last_name='" . addslashes($sup_last_name) . "'
        AND ce.department_id='" . $diff_row["company_department_id"] . "'
        ");
    $result2 = $GLOBALS['dbh']->Execute($sql);

    if ($result2->RecordCount() > 1)
    {
        // We got multiple results back - try to narrow it down using the e-mail address
        $sql = ("
            SELECT DISTINCT c.contact_id
            FROM contact AS c
            INNER JOIN contact_employer AS ce
            ON ce.contact_id=c.contact_id
            WHERE c.first_name='" . addslashes($sup_first_name) . "'
            AND c.last_name='" . addslashes($sup_last_name) . "'
            AND ce.department_id='" . $diff_row["company_department_id"] . "'
            AND c.email='" . addslashes($diff_row["supervisor_email"]) . "'
            ");
        $result2 = $GLOBALS['dbh']->Execute($sql);

        if ($result2->RecordCount() > 1)
        {
            $sql = ("
                SELECT c.contact_id,
                IF (c.phone = '".USE_COMPANY."'
                    , ec.phone
                    , IF (c.phone = '".USE_DEPARTMENT."' AND ed.phone = '".USE_COMPANY."'
                        , ec.phone
                        , IF (c.phone = '".USE_DEPARTMENT."'
                            , ed.phone
                            , c.phone)))
                AS c_phone
                FROM contact AS c
                INNER JOIN contact_employer AS ce
                ON ce.contact_id = c.contact_id
                LEFT JOIN employer_department ed
                ON ed.department_id = ce.department_id
                LEFT JOIN employer_company ec 
                ON ed.employer_id = ec.employer_id
                WHERE c.first_name = '".addslashes($sup_first_name)."'
                AND c.last_name = '".addslashes($sup_last_name)."'
                AND ce.department_id = '".$diff_row['company_department_id']."'
                AND c.email = '".addslashes($diff_row['supervisor_email'])."'
                ");
            $phone_result = $GLOBALS['dbh']->Execute($sql);

            if ($phone_result->RecordCount())
            {
                $arrContactID = array();
                
                while($phone_row = $phone_result->FetchRow())
                {
                    if ($phone_row['c_phone'] == $diff_row['supervisor_phone_number'])
                    {
                        array_push($arrContactID,$phone_row['contact_id']);
                    }
                }
                if (sizeof($arrContactID) == 1)
                {
                    $temp_supervisor_contact = $arrContactID[0];
                }
            }
        }
        elseif ($result2->RecordCount() == 1)
        {
            // Assign the contact_id to a holding variable.
            $row2 = $result2->FetchRow();
            $temp_supervisor_contact = $row2["contact_id"];
        }
        else
        {
            // No results.  Try searching just using the phone.
            $sql = ("
                SELECT c.contact_id,
                IF (c.phone = '".USE_COMPANY."'
                    , ec.phone
                    , IF (c.phone = '".USE_DEPARTMENT."' AND ed.phone = '".USE_COMPANY."'
                        , ec.phone
                        , IF (c.phone = '".USE_DEPARTMENT."'
                            , ed.phone
                            , c.phone)))
                AS c_phone
                FROM contact AS c
                INNER JOIN contact_employer AS ce
                ON ce.contact_id = c.contact_id
                LEFT JOIN employer_department ed
                ON ed.department_id = ce.department_id
                LEFT JOIN employer_company ec 
                ON ed.employer_id = ec.employer_id
                WHERE c.first_name = '".addslashes($sup_first_name)."'
                AND c.last_name = '".addslashes($sup_last_name)."'
                AND ce.department_id = '".$diff_row['company_department_id']."'
                ");
            $phone_result = $GLOBALS['dbh']->Execute($sql);

            if ($phone_result->RecordCount())
            {
                $arrContactID = array();
                
                while($phone_row = $phone_result->FetchRow())
                {
                    if ($phone_row['c_phone'] == $diff_row['supervisor_phone_number'])
                    {
                        array_push($arrContactID,$phone_row['contact_id']);
                    }
                }
                // Make sure we only get one result back. If we get one back, then we've
                // got a contact_id we can use.
                if (sizeof($arrContactID) == 1)
                {
                    $temp_supervisor_contact = $arrContactID[0];
                }
            }
        }
    }
    elseif ($result2->RecordCount() == 1)
    {
        // Exactly one result, assign it to a holding variable.
        $row2 = $result2->FetchRow();
        $temp_supervisor_contact = $row2["contact_id"];
    }
    else
    {
        // Do nothing, if the contact doesn't have a matching name, it likely isn't in our database.
        $temp_supervisor_contact = 0;
    }

    return $temp_supervisor_contact;
}
// }}}
// {{{ function updateHistorySupervisorContactID($contact_id) 

/**
 * When a new contact is added to the database, we want to match supervisor names to this new contact's name.
 * This is done such that the new contact gets credited (status-wise) of the job placement. 
 */
function updateHistorySupervisorContactID($contact_id) 
{
    if (!$contact_id)
    {
        return -1;
    }

    $division_id = getContactsDepartmentID($contact_id);

    $sql = ("
        SELECT history_id
        FROM history
        WHERE company_department_id = '".$division_id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount() ==  0) 
    {
        return;
    }

    $matched_contact_flag = 0;

    while($row = $result->FetchRow())
    {
        $matched_contact_id = getSupervisorContactID($row['history_id']);

        if ($matched_contact_id == $contact_id)
        {
            $matched_contact_flag = 1;

            $update_sql = ("
                UPDATE history 
                SET temp_supervisor_contact = '".$contact_id."'
                WHERE history_id = '".$row['history_id']."'
                ");
            $GLOBALS['dbh']->Execute($update_sql);
        }
    }

    // If we manage to match a supervisor with a contact, we need to give that contact credit by updating
    // its status. So, we return the attributes of the history record that are relevent for contact statuses. 
    // We return the latest history record because this function is only called when new contacts are added
    // to the database, and we only care about their latest activities. 
    if ($matched_contact_flag)
    {
        $sql = ("
            SELECT history_id, term_id, year, temp_date_entered
            FROM history
            WHERE temp_supervisor_contact = '".$contact_id."'
            ORDER BY temp_date_entered DESC 
            LIMIT 0,1
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        
        $arrResult = array();

        $arrResult['history_id'] = $row['history_id'];
        $arrResult['term_id'] = $row['term_id'];
        $arrResult['year'] = $row['year'];
        $arrResult['temp_date_entered'] = $row['temp_date_entered'];
        
        return $arrResult; 
    }
}
// }}}
// {{{ function getTempDateEntered($term_id, $year)

/**
 * This function is used for the automated contact statuses. When a job/history record is entered, we have 
 * to determine when they were entered into the system. We always post jobs/history records for next term, so
 * we look at the term_id and year of the job/history record, and see if it is next term. If it is, it was entered
 * today. Otherwise, if this job/history record was entered for future or past terms, we pro-rate the date entered.
 * We take the start date of the provided term, and subtract 2 months from it, and that is the date the job/history
 * record was entered. 
 */
function getTempDateEntered($term_id, $year)
{
    $arrNextTerm = getNextTermYear();

    // If provided term, year is same as next term, then today is the temp date entered. 
    if ($term_id == $arrNextTerm["term"] && $year == $arrNextTerm["year"])
    {
       $temp_date_entered = date("Y-m-d"); 
    }
    // Otherwise, it's the term minus 2 months. 
    else
    {
        $sql = ("
            SELECT start_date
            FROM term
            WHERE term_id = '".$term_id."'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        
        if ($result->RecordCount())
        {   
            $row = $result->FetchRow();

            $start_date = $row["start_date"];

            // Get the month number that is 2 months before the current one, and adjust the year if necessary.
            $start_date -= 2;
            if ($start_date < 1)
            {   
                $start_date += 12;
                $year -= 1;                                                                                                                          
            }                                                                                                                                        

            $temp_date_entered = date("Y-m-d", mktime(0, 0, 0, $start_date, 1, $year));                                                              

        }
        // Else the provided term isn't a valid term in the system. 
        else
        {
            $temp_date_entered = -1;
        }
    }

    return $temp_date_entered;
}
// }}}
// {{{ function updateContactStatus($contact_id, $department_id, $job_id, $history_id) 

/**
 * An activity has occurred for a contact, so we must update his/her status. This function is the main function to call for dynamic statuses.
 * If both job_id and history_id is null, it means a contact has been added to the database, give him/her a contact potential status. Otherwise
 * if history or job ids have been provided, an activity has happened, figure out their statuses and record it. 
 */
function updateContactStatus($contact_id, $department_id, $job_id, $history_id) 
{
    // Both contact_id and department_id are required fields. 
    if (!$contact_id || !$department_id)
    {
        return -1;
    }

    // We're updating a contact's status because of a job posting or placement
    if ($job_id || $history_id)
    {
        // Grab information about the job or history record. 
        if ($job_id)
        {
            $sql = ("
                SELECT temp_date_entered, term_id, year
                FROM job_info
                WHERE job_id = '".addslashes($job_id)."'
                ");
        }
        elseif ($history_id)
        {
            $sql = ("
                SELECT temp_date_entered, term_id, year
                FROM history
                WHERE history_id = '".addslashes($history_id)."'
                ");
        }

        $result = $GLOBALS['dbh']->Execute($sql);

        // Query was valid, pull out the job/history's activity date and term_id and year. 
        if ($result->RecordCount())
        {
            $row = $result->FetchRow();
            $activity_date = $row['temp_date_entered'];
            $term_id = $row['term_id'];
            $year = $row['year'];

            // Set contact status
            $arrContactStatusResult = getNextContactStatus($contact_id, $department_id, $term_id, $year);
            $status_id = $arrContactStatusResult['status'];

            $arrTermYear = getNextTermYear();

            if ($term_id == $arrTermYear['term'] && $year == $arrTermYear['year']) 
            {
                $activity_date = date("Y-m-d");
            }
        }
        // We have no rows back from the database even when we ran a query, something's wrong.
        elseif ($sql)
        {
            return -1;
        }

        $sql = null;

        if ($history_id)
        {
            $sql = ("
                INSERT INTO department_contact_status
                (contact_id, department_id, status_id, entered_on, activity_type_id, history_id, term_id, year, activity_date)
                VALUES
                ('".addslashes($contact_id)."', '".addslashes($department_id)."','".$status_id."', now(), '".ACTIVITY."','".addslashes($history_id)."'
                ,'".$term_id."','".$year."', '".$activity_date."')
                ");

        }
        elseif ($job_id)
        {
            $sql = ("
                INSERT INTO department_contact_status
                (contact_id, department_id, status_id, entered_on, activity_type_id, job_id, term_id, year, activity_date)
                VALUES
                ('".addslashes($contact_id)."', '".addslashes($department_id)."','".$status_id."', now(), '".ACTIVITY."','".addslashes($job_id)."'
                ,'".$term_id."','".$year."', '".$activity_date."')
                ");
        }

        if ($sql)
        {
            $GLOBALS['dbh']->Execute($sql);

            // Re-evaluate occurs when an older job/history record is entered into the system. If an old status is put into the contact status table while
            // other statuses already exist, we have to reevaluate to ensure our current status is correct. 
            if ($arrContactStatusResult['reevaluate'])
            {
                $status_array = reevaluateContactStatus($contact_id,$department_id);

                $sql = ("
                    SELECT status_id, activity_date
                    FROM department_contact_status
                    WHERE contact_id = '".$contact_id."' AND department_id = '".$department_id."' AND current_status = '1'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();

                // If the current status is not the same as the re-evaluated status, then put it into the database.
                if ($status_array['status'] != $row['status_id'])
                {
                    $sql = ("
                        INSERT INTO department_contact_status
                        (contact_id, status_id, department_id, entered_on, activity_date, activity_type_id, term_id, year)
                        VALUES (
                            '".$contact_id."'
                            , '".$status_array["status"]."'
                            , '".$department_id."'
                            , now()
                            , '".$row["activity_date"]."'
                            , '".AUTOMATED."'
                            , '0'
                            , '0000'
                            )
                        ");
                    $GLOBALS['dbh']->Execute($sql);
                }
            }
        }
    }

    // Otherwise, we're updating a contact status because it's a new contact 
    else
    {
        // if we manage to retroactively find that this contact was a supervisor for a history record, we want to credit the contact.
        $arrStatusInfo = updateHistorySupervisorContactID($contact_id);

        $arrContactStatus = reevaluateContactStatus($contact_id,$department_id);
        $activity_date = $arrContactStatus['activity_date'];

        // So if we get something back from updateHistorySupervisorContactID, it means that this new contact shouldn't have potential lead
        // as he/she has been a supervisor.
        if (is_array($arrStatusInfo) && $arrContactStatus['status'] == CONTACT_ACTIVE_NEW)
        {
            $sql = ("
                INSERT INTO department_contact_status
                (contact_id, department_id, status_id, entered_on, activity_type_id, history_id, term_id, year, activity_date)
                VALUES
                ('".addslashes($contact_id)."', '".addslashes($department_id)."','".$arrContactStatus['status']."', now(), '".ACTIVITY."'
                , '".$arrStatusInfo['history_id']."','".$arrStatusInfo['term_id']."','".$arrStatusInfo['year']."', '".$arrStatusInfo['temp_date_entered']."')
                ");
        }
        else
        {
            $sql = ("
                INSERT INTO department_contact_status
                (contact_id, department_id, status_id, entered_on, activity_type_id, term_id, year, activity_date)
                VALUES
                ('".addslashes($contact_id)."', '".addslashes($department_id)."','".$arrContactStatus['status']."', now(), '".AUTOMATED."'
                ,'0','0000', '".$activity_date."')
                ");

        }

        $GLOBALS['dbh']->Execute($sql);
    }
    
    // Set the latest contact status to be the current status. 
    updateContactCurrentStatus($contact_id, $department_id);

    // Set the division status
    updateDivisionStatus(getContactsDepartmentID($contact_id), $department_id);

    // Set the company status
    updateCompanyStatus(getContactsEmployerID($contact_id), $department_id);

    return 1; 
}
// }}}
// {{{ function getContactsDepartmentID($contact_id)

/**
 * Returns the contact's department_id
 */
function getContactsDepartmentID($contact_id)
{
    $department_id = 0;
    
    $sql = ("
        SELECT department_id
        FROM contact_employer
        WHERE contact_id = '".addslashes($contact_id)."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount())
    {
        $row = $result->FetchRow();
        $department_id = $row['department_id'];
    }
    else
    {
        $department_id = -1;
    }

    return $department_id;
}
// }}}
// {{{ function getContactsEmployerID($contact_id)

/**
 * Returns the contact's employer_id
 */
function getContactsEmployerID($contact_id)
{
    $employer_id = 0;
    
    $sql = ("
        SELECT employer_id
        FROM contact_employer
        WHERE contact_id = '".addslashes($contact_id)."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount())
    {
        $row = $result->FetchRow();
        $employer_id = $row['employer_id'];
    }
    else
    {
        $employer_id = -1;
    }

    return $employer_id;
}
// }}}
// {{{ function updateContactCurrentStatus($contact_id, $department_id)

/**
 * Looks at a contact's status, and takes the status with the greatest activity date and sets it as the current status. 
 */
function updateContactCurrentStatus($contact_id, $department_id)
{
    if (!isInteger($contact_id) || !isInteger($department_id))
    {
        return 0;
    }

    // We have to lock the tables because statuses have to be updated quickly, and we don't want concurrent
    // queries to clobber the following update statements.
    $sql = ("
        LOCK TABLES department_contact_status WRITE
        ");
    $GLOBALS['dbh']->Execute($sql);

    // Bring me back latest status for a contact for a specific department.
    $sql = ("
        SELECT department_contact_status_id, status_id, current_status
        FROM department_contact_status
        WHERE contact_id = '".$contact_id."' and department_id = '".$department_id."'
        ORDER BY activity_date DESC, entered_on DESC
        LIMIT 0,1
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    // If top status has current status = 1, no need to update status. Do nothing.
    // Otherwise the latest status is not the current status, do the following below.  
    if (!$row['current_status'])
    {
        // Make the old status not current
        $update_sql = ("
            UPDATE department_contact_status
            SET current_status = '0'
            WHERE department_id = '".$department_id."' AND contact_id = '".$contact_id."' AND current_status = '1'
            ");
        $GLOBALS['dbh']->Execute($update_sql);

        // Make the latest status current. 
        $update_sql = ("
            UPDATE department_contact_status
            SET current_status = '1'
            WHERE department_contact_status_id = '".$row['department_contact_status_id']."'
            ");
        $GLOBALS['dbh']->Execute($update_sql);
    }
    
    $sql = ("
        UNLOCK TABLES 
        ");
    $GLOBALS['dbh']->Execute($sql);
    return 1;
}
// }}}
// {{{ function updateDivisionCurrentStatus($division_id, $department_id)

/**
 * Looks at a division's status, and takes the status with the greatest activity date and sets it as the current status. 
 */
function updateDivisionCurrentStatus($division_id, $department_id)
{
    if (!isInteger($division_id) || !isInteger($department_id))
    {
        return 0;
    }

    // We have to lock the tables because statuses have to be updated quickly, and we don't want concurrent
    // queries to clobber the following update statements.
    $sql = ("
        LOCK TABLES department_division_status WRITE
        ");
    $GLOBALS['dbh']->Execute($sql);

    // Bring me back latest status for a contact for a specific department.
    $sql = ("
        SELECT department_division_status_id, status_id, current_status
        FROM department_division_status
        WHERE division_id = '".$division_id."' and department_id = '".$department_id."'
        ORDER BY entered_on DESC
        LIMIT 0,1
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    // If top status has current status = 1, no need to update status. Do nothing.
    // Otherwise the latest status is not the current status, do the following below.  
    if (!$row['current_status'])
    {
        // Make the old status not current
        $update_sql = ("
            UPDATE department_division_status
            SET current_status = '0'
            WHERE department_id = '".$department_id."' AND division_id = '".$division_id."' AND current_status = '1'
            ");
        $GLOBALS['dbh']->Execute($update_sql);

        // Make the latest status current. 
        $update_sql = ("
            UPDATE department_division_status
            SET current_status = '1'
            WHERE department_division_status_id = '". $row['department_division_status_id'] ."'
            ");
        $GLOBALS['dbh']->Execute($update_sql);
    }
    
    $sql = ("
        UNLOCK TABLES 
        ");
    $GLOBALS['dbh']->Execute($sql);
    return 1;
}
// }}}
// {{{ function updateCompanyCurrentStatus($employer_id, $department_id)

/**
 * Looks at the greatest activitiy date of a company's status, and sets it as the current status.
 */
function updateCompanyCurrentStatus($employer_id, $department_id)
{
    if (!isInteger($employer_id) || !isInteger($department_id))
    {
        return 0;
    }

    // We have to lock the tables because statuses have to be updated quickly, and we don't want concurrent
    // queries to clobber the following update statements.
    $sql = ("
        LOCK TABLES department_company_status WRITE
        ");
    $GLOBALS['dbh']->Execute($sql);

    // Bring me back latest status for a contact for a specific department.
    $sql = ("
        SELECT department_company_status_id, status_id, current_status
        FROM department_company_status
        WHERE employer_id = '".$employer_id."' and department_id = '".$department_id."'
        ORDER BY entered_on DESC
        LIMIT 0,1
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    // If top status has current status = 1, no need to update status. Do nothing.
    // Otherwise the latest status is not the current status, do the following below.  
    if (!$row['current_status'])
    {
        // Make the old status not current
        $update_sql = ("
            UPDATE department_company_status
            SET current_status = '0'
            WHERE department_id = '".$department_id."' AND employer_id = '".$employer_id."' AND current_status = '1'
            ");
        $GLOBALS['dbh']->Execute($update_sql);

        // Make the latest status current. 
        $update_sql = ("
            UPDATE department_company_status
            SET current_status = '1'
            WHERE department_company_status_id = '". $row['department_company_status_id'] ."'
            ");
        $GLOBALS['dbh']->Execute($update_sql);
    }
    
    $sql = ("
        UNLOCK TABLES 
        ");
    $GLOBALS['dbh']->Execute($sql);
    return 1;
}
// }}}
// {{{ function wasEverEligible($record_id, $term_id, $year)

function wasEverEligible($record_id, $term_id, $year)
{
    $sql = ("
        SELECT record_id
        FROM eligible_ever 
        WHERE record_id = '".$record_id."' AND term_id = '".$term_id."' AND year = '".$year."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    return $result->RecordCount();
}
// }}}
// {{{ function setEverEligible($record_id, $term_id, $year)

function setEverEligible($record_id, $term_id, $year)
{
    $sql = ("
        INSERT INTO eligible_ever 
        (record_id, term_id, year) VALUES 
        ('".$record_id."','".$term_id."','".$year."')
        ");
    $GLOBALS['dbh']->Execute($sql);
}
// }}}
// {{{ function reactivateStudentJobApplication($job_id, $student_number)

/**
 * Used when we want to make a student eligible again for the term, we have to reactivate that student's job applications.
 */
function reactivateStudentJobApplication($job_id, $student_number)
{

    $app_status = null; 
    $apply_method = null;
    $job_has_closed = null;

    // the if clause inside the sql statement checks to see if the closing date has already passed the current time 
    $sql = ("
        SELECT ji.apply_method, IF (CONCAT(closing_date, ' ', closing_time) < NOW(), 1, 0) as job_has_closed
        FROM job_info ji
        WHERE ji.job_id = '".$job_id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $apply_method = $row['apply_method'];
    $job_has_closed = $row['job_has_closed'];
    
    // we only reactivate statuses that use the online method of applying

    if ($apply_method == JOB_APP_METHOD_ONLINE) 
    {
        // if the job hasn't closed yet, we set the application status to an applied mode. 
        if (!$job_has_closed) 
        {
            $app_status = APP_APPLIED_ONLINE;
        }
        else 
        {
            //:TODO: When we implement the job applications sent bit in the job_info table, 
            // we need to update this algorithm chunk here. Right now we're doing the best 
            // guess of whether an online application should be sent or not. 
            $sql = ("
                SELECT count(*) as num_online_sent
                FROM applications
                WHERE job_id = '".$job_id."' AND app_status = '".APP_ACTIVATED_ONLINE."'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $num_online_sent = $row['num_online_sent'];

            // if we have sent online applications, assume this status is sent, otherwise set it to applied
            if ($num_online_sent > 0) {
                $app_status = APP_ACTIVATED_ONLINE;
            }
            else {
                $app_status = APP_APPLIED_ONLINE;
            }

            // if we're saying this application is applied online, we want to double check to
            // see if there are any other applications. If there's only one other applicant, the
            // algorithm above could fail us. So to be safer than sorry, we will make the student's
            // application activated online IFF there's one applicant to the job. 
            if ($app_status == APP_APPLIED_ONLINE) {
                $sql = ("
                    SELECT count(*) as num_applications
                    FROM applications
                    WHERE job_id = '".$job_id."'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $num_applications = $row['num_applications'];
                if ($num_applications == 1) {
                    $app_status = APP_ACTIVATED_ONLINE;
                }
            }
        }

        // if we managed to compute an application status, then update the application 
        if (isset($app_status)) {
            $update_sql = ("
                UPDATE applications
                SET app_status = '".$app_status."'
                WHERE job_id = '".$job_id."' AND student_number = '".$student_number."'
                ");
            $GLOBALS['dbh']->Execute($update_sql);
        }
    } // end if
}
// }}}
// }}}
?>
