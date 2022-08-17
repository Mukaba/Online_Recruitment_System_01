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
 | Filename: mamook.php                                                        |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/
// include('misc/userErrorHandler.inc');
ob_start('ob_gzhandler', 2048);
// stupid IE workaround, PDFs don't save over SSL for stupid IE
if ($PDF) {
    session_cache_limiter("must-revalidate");
}
session_start();
define("DEBUG", false);  // do a `tail -f logs/test.txt` to watch the debugging output
define("DEBUG_QUERY_TRACE", false);
if (DEBUG) { include("misc/debug_functions.inc"); }

if (DEBUG)
{  // *** DEBUG CODE BELOW ***
	dumpToLog("*** BEGIN mamook.php at " . date("Y-m-d H:i:s") . " ***\n");
	dumpToLog("INITIAL COOKIE VALUES {\n");
	dumpCookies();
	dumpToLog("}\n");
}  // *** DEBUG CODE ABOVE ***

// Filename:  mamook.php
// this is the main code for the information sign up system.  The code in this file determines whether or not user
// has logged in, if the user is an administrator or a student, and if the user has selected a menu bar item.  If 
// the user has selected a menu bar item, code from another file is included to perform the selected function.  

include('misc/compatibility.inc');
/* User Defined Error Handling */
// include('misc/userErrorHandler.inc');
// include('misc/userErrorHandler.inc');

// *** constants, functions, configuration, etc  ***
$select=urldecode($select);
$PHP_SELF_MENU = "mamook.php";

$PHP_SELF = $PHP_SELF_MENU . "?menu_select=" . urlencode($menu_select);
include('misc/release_constants.inc'); // release_constants to be included before constants.inc
include('misc/constants.inc');	// constants must be included before wwwclient.inc
include('misc/names.inc');
include('misc/functions.inc');
include('misc/wwwclient.inc');
include(PATH_ADODB . 'adodb.inc.php');
include('misc/db.inc');
include('plugins/plugin_control.inc.php');
include('misc/resume_functions.inc');
include('misc/special_flags_functions.inc');
$client = new wwwclient();

$GLOBALS['plugin_control']->LoadPluginsByType("*");

/*
 If the user is userlevel MULTIPLE and they have selected a department_choice, we need
 to set that as a cookie.
*/

include("misc/authenticate.inc");
$auth = new authenticate_class();

if ($userlevel == MULTIPLE && $department_choice) {
    $_SESSION['SESSION_department'] = $department_choice;
}
// If someone is trying to login as a student using the quickmenu, we need to set up the variables here.
if ($PDF && $select == 'login_as_student') 
{
    //:TODO: limit user level to this
    unset($PDF);
    $select = 'student_login';
    $admin_student_login = 'yes';
    $submit = 'Log In';
    $sql = ("
        SELECT student_number
        FROM student_department
        WHERE record_id='" . $record_id . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $student_number_login = $row["student_number"];
}

if ($admin_student_login == 'yes')
{
    // Test to make sure that the student the user is trying to login using has not withdrawn from all of their departments.
    // :CAUTION: We need to first determine all of the departments that are in the admin user's group, then use this in the
    // query below.
    $depts_in_group = department_groups($_SESSION['SESSION_department']);

    // Make sure we have an array, and that it has a size.
    if (is_array($depts_in_group) && sizeof($depts_in_group))
    {
        foreach($depts_in_group as $dig)
        {
            $dept_string .= "'" . $dig . "',";
        }
        // Trim the trailing comma
        $dept_string = substr($dept_string, 0, -1);
    }
    else
    {
        // This should NEVER occur!
        assert(false);
    }

    $sql = ("
        SELECT DISTINCT sd.record_id
        FROM student_department AS sd
        WHERE (sd.withdraw='' OR sd.withdraw IS NULL)
        AND sd.department_id IN (" . $dept_string . ")
        ");

    // They've used either the text input or the select box, so determine which, and include the appropriate clause in the query
    if ($student_number_login)
    {
        $sql .= ("
            AND sd.student_number='" . $student_number_login . "'
        ");
    }
    else
    {
        $sql .= ("
            AND sd.student_number='" . $student_number_login2 . "'
            ");
    }

    $result = $GLOBALS['dbh']->Execute($sql);
    if (!$result->RecordCount())
    { 
        $error = ("The student you have tried to log in as is either not a student in your department, or has been withdrawn from all co-op departments, and so you may not log in as them.");
        $admin_student_login = "";
        $select = 'student_login';
    }
}
if ($select == "logout")
{
	$auth->logout();  // logout is in misc/authenticate.inc
}

$auth->authenticate();
$userlevel = $auth->userlevel;
$login_id = $auth->login;
$student_number = -1;

if (DEBUG)
{  // *** DEBUG CODE BELOW ***
	dumpToLog("POST AUTHENTICATION CONDITIONS {\n");
	dumpToLog("\tUserlevel: " . $auth->userlevel . "\n");
	dumpToLog("\tLogin: " . $auth->login . "\n");
	dumpToLog("\tDepartment: " . $auth->department . "\n");
	dumpToLog("}\n");
}  // *** DEBUG CODE ABOVE ***


// Set up Department Information for logged on use
$departments_in_group = department_groups($auth->department);

// Select color scheme wanted for the user
include('misc/colorscheme.inc');

// Determine which menubar to show to the user based on the userlevel assigned to them by authenticate.
switch($userlevel)
{
	case HREMPLOYER:
	case EMPLOYER:
		$sql = ("SELECT a.employer_id, a.contact_id, b.department_id
			FROM employer_login AS a, contact_employer AS b
			WHERE a.login_id='" . $auth->login . "'
			AND b.contact_id=a.contact_id");
		
		$result = $GLOBALS['dbh']->Execute($sql);
		
		if ($row = $result->FetchRow()) 
        {
			$employer_id = $row["employer_id"];
			$contact_id = $row["contact_id"];
			$department_id = $row["department_id"];
		}
		else 
        {
			// probably wanna put checking in here
		}
	
		if (!$select)
		{
			$select = "welcome";
		}
		break;

	case OFFICE:
	case TRAFFICDIRECTOR:
		// if user is admin, show admin menu bar
		if (!$select)
		{
			// if user hasn't selected from the menu bar, show welcome screen
			$select = "welcome";
		}
		break;

	case ADMINEMPLOYER:
		// Login as AdminEmployer, choose from list after throwing in search criteria
		$checkExists = $GLOBALS['dbh']->Execute ("
            SELECT login_id 
            FROM employer_login 
            WHERE contact_id='$employer_contact_id_login'
            ");

        $sql = ("
            SELECT employer_id, contact_id, department_id
            FROM contact_employer
            WHERE contact_id='" . $employer_contact_id_login . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        $employer_id = $row["employer_id"];
        $contact_id = $row["contact_id"];
        $department_id = $row["department_id"];

		if ($checkExists->RecordCount() > 0)
        {
			$PHP_SELF .= "&amp;admin_employer_login=yes&amp;employer_contact_id_login=$employer_contact_id_login";
        }
		else 
        {
			$userlevel=$auth->userlevel=OFFICE;
			$error = "Contact is not currently registered as having a login id; therefore, you can not be logged-in as them.";
		}
		
		if (!$select || ($select=="employer_login" && $error==""))
        {  
			$select="welcome";
        }

		break;

	case ADMINSTUDENT:
		// Login as Adminstudent, 3 ways to select, student login, student number, or pulldown (student_number is sent).
		// Priority is now sent to the pulldown, next the student login, then the student number field.
		if (trim($student_login_id) != "" && trim($student_number_login2) == "")
        {
			$result = $GLOBALS['dbh']->Execute("
                SELECT student_number
                FROM student
                WHERE login_id = '" . $student_login_id . "'
                AND department_id IN ('" . implode("', '", $departments_in_group) . "')");
		}
        else
        {
			if(trim($student_number_login) == "") 
            {
				$student_number_login = $student_number_login2;
			}
			$result = $GLOBALS['dbh']->Execute("
                SELECT s.student_number 
                FROM student as s ,student_department as sd 
                WHERE s.student_number = '" . $student_number_login . "' 
                AND sd.student_number=s.student_number 
                AND sd.department_id IN ('" . implode("', '", $departments_in_group) . "')
                ");
		}

		if ($result->RecordCount() >= 1)
		{
			$row = $result->FetchRow();
			$student_number = $row["student_number"];
            $GLOBALS['student_number'] = $row['student_number'];
			$PHP_SELF .= "&amp;admin_student_login=yes&amp;student_number_login=$student_number";
		}
        else
        {
			$userlevel=$auth->userlevel=OFFICE;
			if ($student_login_id != "")
            {
                $error = "Invalid userID";
            }
            else
            {
                $error = "Invalid Student Number";
            }
		}

		if (!$select || ($select=="student_login" && $error=="") )
		{
			$select="welcome";
		}
		break;
	case STUDENT:
		if ($_SESSION['SESSION_netlog'] == SCRIPT_LOGIN)
        {
			$result = $GLOBALS['dbh']->Execute("
                SELECT student_number 
                FROM student 
                WHERE netlink_id='" . addslashes($login_id) . "'");
		} 
        else 
        {
			$result = $GLOBALS['dbh']->Execute("
                SELECT student_number 
                FROM student 
                WHERE login_id='" . addslashes($login_id) . "'");
		}

		if ($result->RecordCount() == 1)
		{
			$row = $result->FetchRow();
			$student_number = $row["student_number"];
		}
		else
		{
			// ***********
			// ** Note: Might want to add some handling here because if we haven't
			// **       matched a student number, then the student is not in the
			// **       DB (or something worse)
			// **       Alas, this will be caught later on, but it's appropriate
			// **       to stop it here.
			// ***********
		}

		if (!$select)
		{
			// if user hasn't selected from the menu bar, show welcome screen
			$select = "welcome";
		}
		break;

    case FACULTY:
		if (!$select)
		{
			// if user hasn't selected from the menu bar, show welcome screen
			$select = "welcome";
		}
		break;

	case NEW_EMPLOYER:
		// This is a first time employer
		if (!$select)
		{
			$select = "new_employer";
		}
		break;
	
	case FORGOTTEN_PASSWORD:
		// This is an employer who has forgotten her password
		if (!$select)
		{
			$select = "forgotten_password";
		}
		break;
	case LIMBO:
		// password validated, but not belonging to a group with any access
		$menu_bar = $levels[LIMBO];
		$select = "not_in_group";
		break;

	case MULTIPLE:
		// Password validated, but the user belongs to multiple departments.  Let them choose their department.
		$menu_bar = $levels[LIMBO];
		$select = "department_choices";
		break;

	case USERLEVEL_PUBLIC:
        // no password was entered
		if ($authcheck)
		{
			// if user tried to enter system without a password, indicate that the login was invalid, display login sceeen
			$bad_login = "wrong_pass";
			$auth->logout();
			$_SESSION['SESSION_colorscheme'] = DEFAULT_COLORSCHEME;

		} // fall through
        
	default:
        $menu_bar = $levels[USERLEVEL_PUBLIC];
        //This is to make SURE a public user cant go anywhere		
        include('header.inc');
        include('login.inc');
        include('footer.inc');
        ob_end_flush();
        die; // processing ends here.
       
}
assert_options(ASSERT_ACTIVE, TRUE);
assert_options(ASSERT_QUIET_EVAL, TRUE);
/*
 If we are exporting to a file for the user to download.
 The button to export has to have the name "export" (or case there of), which is how it catches
*/
if ($export) 
{ 
	include ('export_file/Export_File.class');
	$file = new ExportTabFile ();
	
	switch ($export) 
    {
		case 'Export Mail Labels':
			$file->EmployerMailLabels ($contact_id_string);
			break;

		default:
            $file->CreateTabFileString(unpackObject($export_string));
			break;
	}
	
    // this is set so we don't fall through way below (where everything is found
	// this means you shouldn't change the "elseif ($PDF)" to just an if, cause it'll screw up
	$PDF = 1;
	// just in case, just to see
	$exported = true;
}
elseif($PDF && !$exported)
{ 
    // If we are displaying something in PDF, then do this now before processing anything else.
    // This switch statement also deals with all quickmenu calls. 
	switch ($select)
    {
        	     case "url_bouncer":
			// Stupid hack that bounces to URLs... makes forms that take people to a predefined place much easier.
			header ("Location: $bounce");
			break;
			
	     case "applications":
			include("applications/apply.inc");
			break;

	     case "coverletter":
			include ("coverletters/coverletter.inc");
			break;

	     case "resume": 
			include ("resume/resume.inc"); 
		  	break;

	     case "transcript":
			include("transcript/transcript.inc");
			break;

	     case "jobOutput":
	     case "edit_job":
            if($show_quick)
            {
                unset($PDF);
            }
            else
            {
                include("PDF/PDFconversion/ConvertToPDF.class");
                $convert = new ConvertToPDF();
                $convert->convert_job(urldecode($HTMLjob));
            }
			break;

	     case "companyOutput":
			include("PDF/PDFconversion/ConvertToPDF.class");
			$convert = new ConvertToPDF();
			$convert->convert_contactinfo(urldecode($HTMLCompany));
			break;

	    case "resume_transcript_output":
            include("PDF/PDFconversion/ConvertToPDF.class");
            if ($userlevel == OFFICE || $userlevel == FACULTY || $userlevel == TRAFFICDIRECTOR) {
                $convert = new ConvertToPDF();
                $resume_sql = ("
                        SELECT resume
                        FROM resume
                        WHERE resume_id='".$resume_id."'
                        ");

                $resume_results = $GLOBALS['dbh']->Execute($resume_sql);
                $resume_row = $resume_results->FetchRow();
                $transcript_sql = ("
                        SELECT transcript
                        FROM transcript
                        WHERE student_number='".$student_num."'
                        ");
                $transcript_results = $GLOBALS['dbh']->Execute($transcript_sql);

                $transcript_row=$transcript_results->FetchRow();

                $HTMLresume_transcript=$resume_row['resume']."<!-- PAGE BREAK -->".$transcript_row['transcript'];

                $convert->convert_contactinfo($HTMLresume_transcript);
            } else {
				include ('misc/loguser.inc');
            }
			break;

	    case "studentOutput":
			include("PDF/PDFconversion/ConvertToPDF.class");
			$convert = new ConvertToPDF();
			$convert->convert_contactinfo(urldecode($HTMLStudent));
			break;

        case "studentApplication":
            include ( "PDF/PDFconversion/ConvertToPDF.class" );
            $covert_class = new ConvertToPDF();
            $covert_class->convert_single_application ($application_id); // :BUG:
            break;

	    case "historyOutput":
			include("PDF/PDFconversion/ConvertToPDF.class");
			$convert = new ConvertToPDF();
			$convert->convert_contactinfo(urldecode($HTMLhistory));
			break;

        case "reportOutput":
			include("PDF/PDFconversion/ConvertToPDF.class");
            $convert = unpackObject(($convertObject));
            $convert->convert_HTMLcontactinfo();
			$convert->send_output();
			break;

        case "set_flags":
            unset($PDF);
            $select = 'set_contact_flag';
            $level1 = 'flags';
            $continue = 'show_specific';
            break;

        case "set_actions":
            unset($PDF);
            $select = 'set_contact_flag';
            $level1 = 'actions';
            $continue = 'show_specific';
            break;

        case "Set Flags for Matched Contacts":
            unset($PDF);
            $select = 'set_contact_flag';
            $level1 = 'flags';
            $contact_id_list_supplied = true;
            $form_submitted = true;
            break;
            
        case "Set Actions for Matched Contacts":
            unset($PDF);
            $select = 'set_contact_flag';
            $level1 = 'actions';
            $form_submitted = true;
            break;
            
        case "history":
            unset($PDF);
            $continue = "View History";
        break;

        case "edithistory":
            unset($PDF);
            $continue = "Edit History";
        break;

        case "view_job":
            unset($PDF);
        break;

        case "view_company_divisions":
            unset($PDF);
            $select = 'view_contact';
            $level1 = 'company';
            $continue = 'company_quickview';
            break;

       case "view_company_contacts":
            unset($PDF);
            $select = 'view_contact';
            $level1 = 'contact';
            $continue = 'company_quickview';
            break;

        case "view_department_contacts":
            unset($PDF);
            $select = 'view_contact';
            $level1 = 'contact';
            $continue = 'department_quickview';
            break;

        case "view_this_contact":
            unset($PDF);
            $select = 'view_contact';
            $level1 = 'contact';
            $continue = 'view_specific_contact';
            $no_buttons = true;
            $show_quick = true;
            break;
        
        case "view_this_company":
        case "view_this_division":
            if($select == "view_this_company")
            {
                unset($department_id);
            }
            unset($PDF);
            $select = 'view_contact';
            $level1 = 'company';
            $continue = 'view_specific_details';
            $no_buttons = true;
            $show_quick = true;
            break;
        
        case "view_this_interview":
            unset($PDF);
            $select = 'view';
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {                
                $level1 = 'job_code';
                /* Commented out: Jon
                   level2 and submit lead to the same place but submit
                   seems safer. Replace if submit doesn't end up working

                   $level2 = 'job_id_or_job_code';
                 */
                $submit = 'View Interview';
                unset($department_id);
            }
            break;
            
        case "add_job_under_company":
        case "add_job_under_division":
            unset($PDF);
            $select = 'add_job';
            break;

        case "add_job_under_contact":
            unset($PDF);
            $select = 'add_job';
           $continue = 'level1';
            if (!$department_id)
            {
                $department_id = 'none';
            }
            break;

        case "edit_contact":
            unset($PDF);
            $select = 'edit_contact';
            $level1 = 'contact';
            break;

	    case "edit_company":
			unset($PDF);
			$select = 'edit_contact';
			$level1 = 'company';
			break;

	    case "edit_department":
			unset($PDF);
			$select = 'edit_contact';
			$level1 = 'department';
			break;

	    case "create_lp":
            unset($PDF);
            $select = 'make_login_info';
			$continue = 'contact_chosen';
			break;

	    case "add_division_to_company":
			unset($PDF);
			$select = 'add_contact';
			$level1 = 'department';
			break;

	    case "add_contact_to_company":
			$department_id = 'none';
			// Drop through to next case statement.

	    case "add_contact_to_division":
			unset($PDF);
			$select = 'add_contact';
			$level1 = 'contact';
			break;

	    case "view_company_history":
			unset($PDF);
			$select = 'employer_history';
            // Get rid of this variable, which may have been set from a quickmenu.  We don't want it set.
            unset($department_id);
			$employer_list = ("('" . $employer_id . "')");
			$Submit = 'View';
			break;

        case "view_division_history":
			unset($PDF);
			$select = 'employer_history';
			$employer_list = ("('" . $employer_id . "')");
			$department_list = $department_id;
			$Submit = 'View';
			break;

        // Student quickmenu calls
        case "edit_student":
            unset($PDF);
            $select = 'edit_student'; // Just to be safe
            $continue = 'chosen_student';
            break;

        case "view_student":
            unset($PDF);
            $select = 'view_student'; // Just to be safe
            $continue = 'Chosen Student';
            break;

         case "student_coverletters":
            unset($PDF);
            if (isset($student_department)) {
                // We're coming from a history quickmenu
                $department_id = $student_department;
            }

            $select = 'coverletter';
            $page = 'View Cover Letters';

            if ($userlevel == STUDENT){
                $student_num = $auth->student_number;
            } else {
                $sql = ("
                    SELECT student_number
                    FROM student_department
                    WHERE record_id='" . $record_id . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $student_num = $row["student_number"];
            }
            break;

        case "student_resumes":
            unset($PDF);
            if (isset($student_department)) {
                // We're coming from a history quickmenu
                $department_id = $student_department;
            }
            
            if ($userlevel == STUDENT){
                $student_num = $auth->student_number;
            } else {
                $select = 'resume';
                $page = 'View Resumes';
                $sql = ("
                    SELECT student_number
                    FROM student_department
                    WHERE record_id='" . $record_id . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $student_num = $row["student_number"];
            }
            break;

        case "student_history":
            unset($PDF);            
            $searchmode = 'simple';
            $search_by_student = 'true';
            $sql = ("
                SELECT student_number
                FROM student_department
                WHERE record_id='" . $record_id . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $student_number_select = $row["student_number"];
            $btnSubmit = 'View Placements';
            break;

        case "student_transcripts":
            if (isset($student_department)) {
                // We're coming from a history quickmenu
                $department_id = $student_department;
            }
            if ($userlevel == STUDENT){
                $student_num = $auth->student_number;
            } else {
                $select = 'transcript';
                $PDF=1;
                $page = 'View HTML';
                $sql = ("
                    SELECT student_number
                    FROM student_department
                    WHERE record_id='" . $record_id . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $student_num = $row["student_number"];
            }
			include ("transcript/transcript.inc" );
            break;

        case "student_interviews": 
            unset($PDF);
            $select = 'view';
            $level1 = 'student';
            $leve2 = 'student_num_or_student_name';
            $department_id='0';
            $submit = 'View Interviews';
            $sql = ("
                SELECT student_number
                FROM student_department
                WHERE record_id='" . $record_id . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $student_num = $row["student_number"];
            break;

        case "place_student_posted_job": 
            unset($PDF);
            $select = 'placement_by_student';
            $sql = ("
                SELECT student_number
                FROM student_department
                WHERE record_id='" . $record_id . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $student_num = $row["student_number"];
            $btnSubmit = 'Submit';
            break;

        case "place_student_own_job":
            unset($PDF);
            $select = 'add_history';
            if(!$student_number)
            {
                $sql = ("
                    SELECT student_number
                    FROM student_department
                    WHERE record_id='" . $record_id . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $student_num = $row["student_number"];
            }
            break;

        case "place_student_returning_job":
            unset($PDF);
            $select = 'returning_placement';
            if(!$student_number)
            {
                $sql = ("
                    SELECT student_number
                    FROM student_department
                    WHERE record_id='" . $record_id . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $student_num = $row["student_number"];
            }
            break;
        
        case "login_as_student":
            break;

        case "log_me_out":
            //They just need to be logged out, then taken to the forgotten password screen.
			unset($PDF);
			$auth->logout();
			$select = 'forgotten_password';
			break;
	    
        case "view_company_job":
        case "view_division_job":
        case "view_contact_job":
            unset($PDF);
            break; 
        	
        default:
            if (!$job_id)
            {
                include('misc/log_form_variables.inc');
            }
            assert ( $job_id );

            if( $selection == 'single' )
            {
                include('interview/edit/EditInterview.class');
                include('PDF/PDFReport/SingleJobReport.class');	
                new SingleJobReport($job_id, $auth->department);
            }
            elseif( $selection == 'final' || $selection == "final_student")
            {
                include('interview/edit/EditInterview.class');
                include('PDF/PDFReport/FinalScheduleReport.class');	

                new FinalScheduleReport($job_id, $auth->department);
            }
            elseif( $selection == 'email_schedule' )
            {
                unset( $PDF );
                $select = 'sign_up';
                $department_id = $auth->department;
                $email_employer = 'create';
            }
            elseif( $selection == 'email_students' )
            {
                unset( $PDF );
                $select = 'view';
                $department_id = $auth->department;
                $email_employer = 'create';
                $level1 = 'email_students';
            }
            elseif( $selection == 'email_students' )
            {
                unset( $PDF );
                $select = 'view';
                $department_id = $auth->department;
                $email_employer = 'create';
                $level1 = 'email_students';
            }
            elseif( $selection == 'view_company' )
            {
                unset($department_id);
                unset($PDF);
                $select = 'view_contact';
                $level1 = 'company';
                $continue = 'view_specific_details';
                $no_buttons = true;
                $show_quick = true;
            }
            elseif( $selection == 'view_applications' )
            {
                unset($PDF);
                $select = 'applications';
                $continue = 'jump_to_view';
            }
            elseif( $selection == 'view_contact' )
            {
                unset($PDF);
                $select = 'view_contact';
                $level1 = 'contact';
                $continue = 'view_specific_contact';
                $no_buttons = true;                             
                $show_quick = true;     
            }
            elseif( $selection == 'view_department' )
            {                               
                unset($PDF);                                
                $select = 'view_contact';
                $level1 = 'department';
                $continue = 'view_specific_details';
                $no_buttons = true;
                $show_quick = true;
            }
            elseif( $selection == 'edit' )
            {
                unset($PDF);
                $department_id = $auth->department;
                $select = 'edit';
            }
            else
            {
                unset( $PDF );
                $select = $selection;
            }
            break;
	} // Switch
} //elseif( $PDF && !$exported)
if($password_changed && !$exported)
{
	/*
	 This happenes when the employer has changed their password.  We need to set the password to
	 equal the new password, and this must be done before we send the header.

	 Check and make sure the password is valid.  If it is, save it in the database, and then change
	 the cookie.  If the password is invalid, unset $password_changed and change_login_info.inc
	 will let the user know their password was invalid.
	*/

	// We've been given a new password.  Validate it (it must be four characters or more) and save it.

    $password1 = removeSlashes($password1);
    $password2 = removeSlashes($password2);

    if (trim($password1) != trim($password2))
    {
        $error = 1;
        $error_msg = ("The two passwords you have entered do not match.  You must enter the same password in both text boxes.");
    }
    elseif (strlen($password1) < 4)
    {
        $error = 1;
        $error_msg = ("The password you have specified is less than 4 characters.  Passwords must be at least 4 characters long.");
    }
    elseif (strlen($password1) > 10)
    {
        // This shouldn't be possible, but just in case.
        $error = 1;
        $error_msg = ("The password you have specified is more than 10 characters.  Passwords must be 10 characters or less.");
    }
    elseif (!preg_match("/^[a-zA-Z0-9!@#$%^&*()]+$/", $password1))
    {
        $error = 1;
        $error_msg = ("The password you have provided contains illegal characters.  Passwords may only consist of alphanumeric 
                characters, and any of the following special characters: !@#$%^&*()");
    }

    if ($error == 0)
    {
        // Save the password in the database, update the cookie, and change_login_info.inc will do the rest.
        if ($auth->contact_id) {
            $sql = ("
                    UPDATE employer_login
                    SET password='" . addslashes(crypt($password1, "EM")) . "'
                    WHERE contact_id='" . $auth->contact_id. "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if (!$GLOBALS['dbh']->Affected_Rows()) {
                $error = 1;
                if (crypt($password1, "EM") == $_SESSION['SESSION_passwd']) {
                    $error_msg = ("The password you entered was the same as your old one, so no changes have been saved.");
                } else {
                    $error_msg = ("An error occured while trying to save your password to the database.  Please try again.");
                }
            }
            else
            {
                // Set the cookie password to the new password so they aren't logged out.
                $crypt_pass = crypt($password1, "EM");
                //setcookie("COOKIE_passwd", $_SESSION['SESSION_passwd'] = $crypt_pass, 0, "/", $auth->domain_for_cookie);
                $_SESSION['SESSION_passwd'] = $crypt_pass;
            }
        }
    }
} // if($password_changed && !$exported)
if(!$PDF && !$exported)
{
	// show Mamook logo and menu bar
	include('header.inc');
    switch($select)
	{
        //Popup replacement stuff
        case "student_chooser":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR){
                include("misc/student_chooser.inc");
            }
			else {
				include ('misc/loguser.inc');
            }
            break;
        case "student_chooser2":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR){
                include("misc/student_chooser2.inc");
            }
			else {
				include ('misc/loguser.inc');
            }
            break;
        case "contact_choose":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR){
                include("misc/contact_choose.inc");
            }
			else {
				include ('misc/loguser.inc');
            }
            break;
        case "company_choose":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR){
                include("misc/company_choose.inc");
            }
			else {
				include ('misc/loguser.inc');
            }
            break;
        case "company_department_choose":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR){
                include("misc/company_department_choose.inc");
            }
			else {
				include ('misc/loguser.inc');
            }
            break;
        case "company_department_choose_2":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR){
                include("misc/company_department_choose_2.inc");
            }
			else {
				include ('misc/loguser.inc');
            }
            break;
        case "company_department_choose_3":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR){
                include("misc/company_department_choose_3.inc");
            }
            break;
        case "company_department_choose_4":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR){
                include ("misc/company_department_choose_4.inc");
            }
            break;
        case "popupChooser":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR){
                include ("misc/popupChooser.inc");
            }
			else {
				include ('misc/loguser.inc');
            }
            break;
        case "student_photo_viewer":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR){
                include ("student_info/student_photo_viewer.inc");
            }
			else {
				include ('misc/loguser.inc');
            }
            break;
        case "popupcalendar":
            include ("popupcalendar/calendar.inc");
            break;
        case "selectbox":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR){
                include ("misc/selectbox.inc");
            }
            break;
        case "redirect":
	        $location="mamook.php?menu_select=1&select=lunch_send_email&cid=".$cid."&time_id=".$time_id;
	        header("Location: $location");
            break;
        //End popup replacement code
        
	    //TODO: This is the link to the new traffic director flag editor	
        case "flag_editor":
            if ($userlevel == TRAFFICDIRECTOR) {
                include ("misc/flag_management/traffic_flag_editor.inc");
            }
			else {
				include ('misc/loguser.inc');
            }
            break;
        
        case "flag_add":
            if ($userlevel == TRAFFICDIRECTOR) {
                include ("misc/flag_management/traffic_flag_add.inc");
            }
			else {
				include ('misc/loguser.inc');
            }
            break;

        // was applications/view/student/applications.inc changed though
		case "applications":
			include ("applications/apply.inc");
			break;

        case "save_search":
            include ("search/save_search.inc");
            break;	
            
		case "manage_searches":
            include ("search/manage_searches.inc");
            break;
        case "make_login_info":
			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR) 
            {
                include('contact/login_info/admin/login_info_header.inc');
            }
			else
            {
				include ('misc/loguser.inc');
            }
            break;

        case "change_login_password":
            include('contact/login_info/employer/login_info_header.inc');
            break;

		case "apply_job":
			// Load up the applications scripts.
			include('applications/apply.inc');
			break;

		case "forgotten_password":
			/* 
             This is an employer that has forgotten their password
			 Check and make sure that they haven't been blacklisted.
            */

			$sql = ("
				SELECT DISTINCT blacklisted_IP
				FROM forgot_password_blacklist
				WHERE blacklisted_IP='" . $REMOTE_ADDR . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			if ($result->RecordCount())
			{
				$error_msg = ("For security reasons, you will now need to contact a ".BRAND_NAME." administrator to retrieve your password.");
				include('login.inc');
			}
			else
			{
				include('contact/login_info/employer/forgotten_password_header.inc');
			}
			break;

		case "new_employer":
			// This is an employers first time logging in.
			include('new_employer/new_employer_header.inc');
			break;

		case "department_choices":
			// Give the user the choice of which department to login under.
			include('misc/department_choices.inc');
			break;
		
        case "welcome":
			// show welcome screen
                $sqldept = ("SELECT DISTINCT department_id FROM department
                        WHERE using_full_system AND department_id = '". $auth->department."'");
                $resultdept = $GLOBALS['dbh']->Execute($sqldept);
                $rowdept=$resultdept->FetchRow();

                if ($userlevel == STUDENT && ($auth->department == $rowdept['department_id']))
                {
                    include("welcome_applicant.inc");
                }
                elseif ($userlevel == EMPLOYER)
                {
                    include("welcome_employer.inc");
                }
                elseif ($userlevel == ADMINEMPLOYER)
                {
                    include("welcome_employer.inc");
                }
                elseif ($userlevel == ADMINSTUDENT && ($auth->department == $rowdept['department_id'])) 
                {
                    include("welcome_applicant.inc");
                }
                else
                {
                    include("welcome.inc");
                }
            break;

		case "logout":
			$logout = 1;
			$select = "";
			break;
	
		case "add":
			// go to add interview screens
			if ($userlevel == OFFICE) 
			{ 
				include("interview/add/add.inc"); 
			} 
			else 
			{ 
				include("misc/loguser.inc"); 
			}
			break;

		case "edit":
			// go to edit interview screens
			if ($userlevel == OFFICE) 
			{ 
				include("interview/edit/edit.inc"); 
			} 
			else 
			{ 
				include("misc/loguser.inc"); 
			}
			break;

		case "room_booking":
			// go to room booking screens
			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR) 
            { 
                include("misc/room/room.inc"); 
            } 
            else 
            { 
                include("misc/loguser.inc"); 
            }
			break;

		case "sign_up":
			// go to the sign up (admin) screens
			if ($userlevel == OFFICE) 
            { 
                include("interview/sign_up/admin/sign_up.inc"); 
            } 
            else 
            { 
                include("misc/loguser.inc"); 
            }
			break;
	
		case "view":
			// new view.
			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR) 
            {
				include("interview/view/view.inc");
            }
			elseif(($userlevel == EMPLOYER) || ($userlevel == ADMINEMPLOYER))
            {
				include("interview/view/employer/view.inc");
            }
			else 
            { 
                include("misc/loguser.inc"); 
            }
			break;

		case "lunch_send_email":
			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR) 
            {
				include("interview/lunch/email/send_lunch_email.inc");
			}
            else
            {
				include ("misc/loguser.inc");
			}
			break;

		case "lunchsome":
			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
				include ("interview/lunch/some.inc");
			}
            else
            {
				include("misc/loguser.inc");
			}
			break;

		case "lunchall":
			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
				include ("interview/lunch/all.inc");	
			}
            else
            {
				include("misc/loguser.inc");
			}
			break;

		case "add_student":
			// if a student doesn't have a login ID, add it to the database;
			if ($userlevel == OFFICE) 
            { 
                include("student_info/add/add_student.inc"); 
            } 
            else 
            { 
                include("misc/loguser.inc"); 
            }
			break;

		case "modify_contact":
			// if a student doesn't have a login ID, add it to the database;
			if ($userlevel == OFFICE) 
			{ 
				include("contact/modify_contact.inc"); 
			} 
			else 
			{ 
				include("misc/loguser.inc"); 
			}
			break;

		case "add_contact":
			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
			{
				include("contact/add/add.inc");
			}
			else
			{
				include("misc/loguser.inc");
			}
			break;

		case "edit_contact":
			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
			{
				include("contact/edit/edit.inc");
			}
			else
			{
				include("misc/loguser.inc");
			}
			break;
		
		case "view_contact":
			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
			{
				include("contact/view/view.inc");
			}
			else
			{
				include("misc/loguser.inc");
			}
        break;

		case "verify_delete_contact":
			if ($userlevel == TRAFFICDIRECTOR)
			{
				include("contact/delete/contact/verify_delete_contact.inc");
			}
			else
			{
				include("misc/loguser.inc");
			}
        break;
        
		case "delete_contact":
			if ($userlevel == TRAFFICDIRECTOR)
			{
				include("contact/delete/contact/delete_contact.inc");
			}
			else
			{
				include("misc/loguser.inc");
			}
        break;

        // :TODO the following mail case statements are violating the way traffic is directed throughout the system. The
        // mail statements bypasses the tree hierarchy established.
        case "send_contact_mail":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                $level1 = 'contact';
                $continue = 'send_contact_mail';
                include("contact/view/view.inc");
            }
            else
            {
                include("misc/loguser.inc");
            }
        break;

        case "set_company_mail":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                $level1 = 'company';
                $continue = 'set_mail';
                include("contact/view/view.inc");
            }
            else
            {
                include("misc/loguser.inc");
            }
        break;
        	
        case "send_company_mail":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                $level1 = 'company';
                $continue = 'send_mail';
                include("contact/view/view.inc");
            }
            else
            {
                include("misc/loguser.inc");
            }
        break;

        case "set_department_mail":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                $level1 = 'company';
                $continue = 'set_mail_department';
                include("contact/view/view.inc");
            }
            else
            {
                include("misc/loguser.inc");
            }
        break;

        case "send_department_mail":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                $level1 = 'company';
                $continue = 'send_mail_department';
                include("contact/view/view.inc");
            }
            else
            {
                include("misc/loguser.inc");
            }
        break;

        case "set_company_search_results_mail":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                $level1 = 'company';
                $continue = 'set_company_search_results_mail';
                include("contact/view/view.inc");
            }
            else
            {
                include("misc/loguser.inc");
            }
        break;

        case "send_company_search_results_mail":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                $level1 = 'company';
                $continue = 'send_company_search_results_mail';
                include("contact/view/view.inc");
            }
            else
            {
                include("misc/loguser.inc");
            }
        break;

        case "set_contact_search_results_mail":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                $level1 = 'contact';
                $continue = 'set_contact_search_results_mail';
                include("contact/view/view.inc");
            }
            else
            {
                include("misc/loguser.inc");
            }
        break;


        case "send_contact_search_results_mail":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                $level1 = 'contact';
                $continue = 'send_contact_search_results_mail';
                include("contact/view/view.inc");
            }
            else
            {
                include("misc/loguser.inc");
            }
        break;

        case "send_department_search_results_mail":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                $level1 = 'company';
                $continue = 'send_department_search_results_mail';
                include("contact/view/view.inc");
            }
            else
            {
                include("misc/loguser.inc");
            }
        break;


        case "search_contact":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                include("contact/search/search.inc");	
            }
            else
            {
				include("misc/loguser.inc");
			}
			break;

		case "set_contact_flag":
			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
			{
				include("contact/set/set.inc");
			}
			else
			{
				include("misc/loguser.inc");
			}
			break;

        case "getting_started":
			if ($userlevel == EMPLOYER || $userlevel == HREMPLOYER || $userlevel == ADMINEMPLOYER)
			{
				include("help/employer/getting_started.inc");
			}
			else
			{
				include("misc/loguser.inc");
			}
            break;

		case "student_sign_up":
			// go to the student sign up screens
			include("interview/sign_up/student/sign_up.inc");
			break;

		case "not_in_group":
			// use/password validated, but not in a group with any rights
			include("misc/not_in_group.inc");
			break;

	   	case "resume":
			include ("resume/resume.inc");
			break;

		case "coverletter":
		 	include ("coverletters/coverletter.inc");
			break;

		case "transcript":
			include ("transcript/transcript.inc" );
			break;

		case "shortlist":
			include( "shortlist/shortlist.inc");
			break;

		case "add_job":
			if ($userlevel == OFFICE || $userlevel == EMPLOYER || $userlevel == TRAFFICDIRECTOR)
            {
                include('job_descriptions/add_job/add.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
            break;

		case "edit_job":
			if ($userlevel == OFFICE || $userlevel == EMPLOYER || $userlevel == TRAFFICDIRECTOR)
            {
                include('job_descriptions/edit_job/edit_job.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;
        
        case "view_company_job":
            $select = 'search_job';
            $show_quick = true;
            $quick_type = 'company_quickview';
            $submit = 'Contact Division Company Search';
            include('job_descriptions/search_job/search.inc');
            break;

        case "view_division_job":
            $select = 'search_job';
            $show_quick = true;
            $quick_type = 'division_quickview';
            $submit = 'Contact Division Company Search';
            include('job_descriptions/search_job/search.inc');
            break;

        case "view_contact_job":
            $select = 'search_job';
            $show_quick = true;
            $quick_type = 'contact_quickview';
            $submit = 'Contact Division Company Search';
            include('job_descriptions/search_job/search.inc');
            break;
        
		case "search_job":
			include('job_descriptions/search_job/search.inc');
			break;

		case "pending_jobs":
			//if ($userlevel == OFFICE  || $userlevel == TRAFFICDIRECTOR)
			if ($userlevel == TRAFFICDIRECTOR)
			{
				include('job_descriptions/pending_jobs/pending_jobs_header.inc');
			}
			else
			{
				include('misc/loguser.inc');
			}
			break;

		case "holding_jobs":
			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
			{
				include('job_descriptions/holding_jobs/holding_jobs.inc');
			}
			else
			{
				include('misc/loguser.inc');
			}
			break;
        
		case "view_job":
            if($level1 == "folders" || $level1 == "Manage Folders")
            {
                include('job_descriptions/folders/folders.inc');
            }
            elseif($job_code)
            {
                $sql = "SELECT DISTINCT job_id ".
                    "FROM job_info ".
                    "WHERE job_code = '".$job_code."'";
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                if($result->RecordCount() !=0)
                {
                    $job_id = $row["job_id"];
                    $show_quick = true;
                    $level1 = "job_id";
                }
                else
                {
                    $false_job_code = true;
                }
			    include('job_descriptions/show_job/show_job.inc');
            }
            else
            {
			    include('job_descriptions/show_job/show_job.inc');
            }
            break;

		case "add_history":
			if ($userlevel == OFFICE)
            {
				include('history/add/add_history_controller.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;

        case "returning_placement":
			if ($userlevel == OFFICE)
            {
				include('history/add/add_history_controller.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;
            

		case "employer_history":
			if ($userlevel == OFFICE)
            {
				include('history/employer/employer_history_choose.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;

		case "email_students":
			if ($userlevel == OFFICE || $userlevel == FACULTY)
            {
				include('student_info/mail.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;

		case "history_email_students":
			if ($userlevel == OFFICE)
            {
				include('history/email.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
        break;
        case "email_contact":
            if ($userlevel == OFFICE)
            {
                include('contact/view/mail.inc');
            }
            else
            {
                include('misc/loguser.inc');
            }
        break;
        case "view_student":
            if ($userlevel == OFFICE || $userlevel == FACULTY)
            {
				include('student_info/view/view_student.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;

		case "edit_student":
			if ($userlevel == OFFICE || $userlevel == FACULTY)
            {
                include('student_info/edit/editstudent.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;
		
		case "results":
			if ($userlevel==OFFICE)
            {
				include('history/simple_search_results.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;

		case "edithistory":
            if ($userlevel==OFFICE)
            {
                $continue = "Edit History";
                include('history/history.inc');
            }
            else
            {
                include('misc/loguser.inc');
            }
            break;

        case "student_history":
            if ($userlevel==OFFICE)
            {
                include('history/choose_search.inc');
            }
            else
            {
                include('misc/loguser.inc');
            }
            break;
        case "history_choose":
            if ($userlevel==OFFICE)
            {
                include('history/choose_search.inc');
            }
            else
            {
                include('misc/loguser.inc');
            }
            break;

		case "history_advanced_search":
			if ($userlevel==OFFICE)
            {
				include('history/advanced_search_form.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;

		case "placement_by_term":
			if ($userlevel == OFFICE)
            {
				include('placement/term/place_by_term_controller.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;

		case "placement_by_student":
			if ($userlevel == OFFICE)
            {
				include('placement/student/place_by_student_controller.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;
		case "verify_history_delete":
			if ($userlevel == OFFICE)
            {
                include('history/verify_delete.inc');
            }
            else
            {
                include('misc/loguser.inc');
            }
			break;

		case "history":
            if ($userlevel == OFFICE)
            {
                $continue = "View History";
                include('history/history.inc');
            }
            else
            {
                include('misc/loguser.inc');
            }
			break;

		case "set_history_flags":
			if ($userlevel == OFFICE)
			{
				include('history/set/set.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;

		case "export_pdf_batch":
            if ($userlevel == OFFICE)
            {
                include('history/export_batch.inc');
            }
            else
            {
                include('misc/loguser.inc');
            }
			break;

		case "delete_record":
			if ($userlevel == OFFICE)
            {
				include('history/delete_history.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;

		case "set_student_flags":
			if ($userlevel == OFFICE)
			{
				include('student_info/set/set.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;

		case "employer_login":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR) {
                include ('misc/employer_login/employer_login.inc');
            }
            else {
                include('misc/loguser.inc');
            }
            break;

		case "student_login":
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR) {
                include('misc/student_login/login_screen.inc');
            }
            else {
                include('misc/loguser.inc');
            }
            break;	

		case "prefer":
			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                include('preferences/personal/preferences.inc');
			}
            elseif ($userlevel == FACULTY)
            {
                include('preferences/personal/preferences_faculty.inc');
            }
            else
            {
                include('misc/loguser.inc');
            }
			break;

		case "prefer_dept":
			if ($userlevel == OFFICE)
            {
                include('preferences/department/preferences.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;
        case "editemployeremailpopup":
            if ($userlevel == OFFICE){
                include('preferences/department/editemployeremailpopup.inc');
            } else {
                include('misc/loguser.inc');
            }
            break;
        case "defaultsemesterspopup":
            if ($userlevel == OFFICE){
                include('preferences/department/defaultsemesterspopup.inc');
            } else {
                include('misc/loguser.inc');
            }
            break;  
		case "prefer_staff":
			if ($userlevel == OFFICE && $_SESSION['SESSION_loginID']==ADMIN_USER_ID)
            {
                include('preferences/staff/preferences.inc');
			}
            else
            {
                include('misc/loguser.inc');
            }
			break;

        case "delete_student":
            if ($_SESSION['SESSION_loginID']==ADMIN_USER_ID) {
                include ('student_info/delete/main.inc');
            } 
            else {
                include ("misc/loguser.inc");
            }
            break;
        case "verify_delete_student":
            if ($_SESSION['SESSION_loginID']==ADMIN_USER_ID) {
                include ('student_info/delete/verify.inc');
            }
            else {
                include ("misc/loguser.inc");
            }
            break;
        case "deleted_student":
            if ($_SESSION['SESSION_loginID']==ADMIN_USER_ID) {
                include ('student_info/delete/deleted.inc');
            }
            else {
                include ("misc/loguser.inc");
            }
            break;
        case "feedback":
            if ($userlevel == OFFICE || $userlevel == ADMINSTUDENT || $userlevel == TRAFFICDIRECTOR 
                || $userlevel == STUDENT || $userlevel == FACULTY)
            {
                include ('misc/feedback.inc');
            }
            else
            {
                include ("misc/loguser.inc");
            }
            break;
            
		default:
			// show the login screen
			if ($select == "")
			{
				include("login.inc");
			}
			else
			{
                $arr_returns = get_hooks('mamook.traffic_control.select', $select);

                if (sizeof($arr_returns)) {
                    break;
                }

				// the $select was something meaningless
				include("welcome.inc");
			}
            break;
		
	} // switch($select)

	include("footer.inc");

	if (DEBUG)
	{  // *** DEBUG CODE BELOW ***
		dumpToLog("*** END mamook.php at " . date("Y-m-d H:i:s") . " ***\n\n");
	}  // *** DEBUG CODE ABOVE ***
} // if(!$PDF && !$exported)
ob_end_flush();
?>
