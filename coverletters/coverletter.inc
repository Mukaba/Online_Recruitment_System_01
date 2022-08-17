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
 | Filename: coverletter.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: Determines what file to include based on the $page and          |
 | $userlevel variables.                                                        |
 +------------------------------------------------------------------------------+

*/

/*
 *
 *General Notes:
 *The way I have set up the coverletter table is as follows:
 *	student_number - this is the student number of the student who owns the coverletter
 *	job_id  - this is the job id corresponding to the coverletter.  If the coverletter is a generic or template coverletter
 *		  then this feild will contain a '0'.
 *	last_modified - this is the date and time that the coverletter was last modified.
 *	type_id - this corresponds to the type of coverletter.  Currently there are three types: SPECIFIC value of 1(corresponds 
 *		  to a specific job), TEMPLATE value of 2 (the template coverletter), or GENERIC values of 3 (the generic coverletter)
 *	status_id - this is the status of the coverletter.  NULL is a status of nothing - the coverletter has been added
 *		    to the system but nothing has happened to it.  1 is ACTIVE (online) - the coverletter is visible by
 *		    employers.  2 is DELETE - the coverletter has been deleted by the administrator.  Currently it is not
 *		    set up to actually delete the coverletters, it just sets the status_id flag to deleted
 *		    This will probably want to be changed in the future.
 *	letter - this is the html string containing the coverletter
 *	include_transcript - this is a flag on whether or not to include the transcript when the application is sent
 *				to the employer.
 *	resume_id - this is the resume id corresponding to the resume that should be attached to the application.
 *
 *The primary key of the table is simply coverletter_id. This was done because of the switch over to applications being seperate from coverletters.
 */

if ($userlevel == EMPLOYER || $userlevel == HREMPLOYER) {

	if ($PDF) {

		switch ($page) {

			case "View All Applications":
				include("PDF/PDFconversion/ConvertToPDF.class");
				$application = new ConvertToPDF();
				$application->process_applications($job_id);
				break;

			case "viewapplication":
				$student = array($student);

			case "View Selected Applications":
				if (sizeof($student) <= 0) {

					unset($PDF);
				}
				
				else {

					include( "PDF/PDFconversion/ConvertToPDF.class" );
					$application = new ConvertToPDF();
					$application->process_selected_applications($job_id, $student);
				}
				break;
			default:
				unset( $PDF );
		}

	}else{
		include( "coverletters/coverletter_header.inc" );
		switch( $page ){
			case "1":
				include( "coverletters/employer/view_applications.inc");
				break;
			default:
				include ("coverletters/employer/view_main.inc");
		}
		include ("coverletters/coverletter_footer.inc");
	}
}elseif ($userlevel == STUDENT || $userlevel == ADMINSTUDENT){
	
	include ('coverletters/student/student_coverletter.inc');

}elseif ($userlevel == OFFICE){
	if ($department_id == ""){

		// this is for multiple departments in a group, to get all those listed otherwise just the one
	        if ($departments_in_group > 1){
	                if ($_SESSION['SESSION_netlog']==SCRIPT_LOGIN) {
	                        $group_sql = "SELECT b.pulldown_menu_group
	                        FROM contact_internal AS b
	                        WHERE b.department_id='$auth->department' AND b.netlink_id='$login_id'";
	                } else {
	                        $group_sql = "SELECT b.pulldown_menu_group
	                        FROM contact_internal AS b
	                        WHERE b.department_id='$auth->department' AND b.login_id='$login_id'";
	                }
	                $result_group=$GLOBALS['dbh']->Execute($group_sql);
	                while($row=$result_group->FetchRow()){
	                        $pulldown_menu_group=$row['pulldown_menu_group'];
	                }
	                if ($pulldown_menu_group=='1'){
	                        $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
	                        $department_id='0';
	                } else {
	                        $department_id = $auth->department;
	                }
		// just one
	        } else {
	                $department_id = $auth->department;
	        }

	}


	//  ?????? not intirely sure what's goin on here --- has to do with applications, so may not need
	if ($department_id != "0" && !in_array($department_id, $departments_in_group)){
		if ($page == "application" ||$page == "View Applications" || $page=="View All Applications" || $page == "View Selected Applications" || $page == "Search Applicants"){
			$page = "application";
		}elseif ($page == "activate" || $page == "Activate"){
			$page = "activate";
		}else{
			$page = "";
		}
		$dep_error = ("You do not have access to this department.");
		$department_id = $auth->department;
	}
	if ($department_id == "0"){
		$department_in_str = "'" . implode("', '", $departments_in_group) . "'";
	}else{
		$department_in_str = "'" . $department_id . "'";
	}


	if( $PDF ){
		switch($page){
			case "View HTML":
				include ("coverletters/admin/view_html.inc");
				break;
			case "View PDF":
				include( "PDF/PDFconversion/ConvertToPDF.class" );
				include( "coverletters/admin/view_pdf.inc" );
				break;
			case "Save Cover Letter":

// don't want anybody saving files just yet, but still need for saving
//			case "Submit File":
				include( "coverletters/save.inc" );
				if ($save_error == ""){
					include( "coverletters/admin/view_html.inc" );
				}else{
					unset( $PDF );
				}
				break;


/* the applications module takes care of this
			case "View All Applications":
				include("PDF/PDFconversion/ConvertToPDF.class");
				$application = new ConvertToPDF();
				$application->process_applications($job_id);
				break;
			case "viewapplication":
				$student = array($student);
			case "View Selected Applications":
				if (sizeof($student) <= 0){
					unset($PDF);
				}else{
					include( "PDF/PDFconversion/ConvertToPDF.class" );
					$application = new ConvertToPDF();
					$application->process_selected_applications($job_id, $student);
				}
				break;
*/
			default:
				unset( $PDF );
		}
	}else{

		// doing some stuff, still not sure what	
		if ($page == "application" || $page == "View Applications" || $page=="View All Applications" || $page == "View Selected Applications" || $page== "Search Applicants" || $page=="activate" || $page == "Activate"){
			include ("coverletters/coverletter_header.inc");
		}else{
			include("coverletters/admin/header.inc");
		}
		if ($dep_error != ""){
			error($dep_error);
		}
		if ($error != ""){
			error($error);
		}elseif ($save_error != ""){
			error($save_error);
		}

		//useful switch to know where we're going
		switch($page){

			case "Search for Job":
			case "Search for Student":
				include( "coverletters/admin/search_main.inc" );
				break;
			case "job search":
			case "student_search":
				include( "coverletters/admin/search.inc");
				break;

			case "Mamook":
			case "View Cover Letters":
			case "viewletters":
				include( "coverletters/admin/view_results.inc" );
				break;
			case "edit":
				include( "coverletters/admin/view_main.inc" );
				break;
			case "add":
				include( "coverletters/admin/add_main.inc" );
				break;
			case "Add Cover Letter":
				include( "coverletters/admin/add.inc" );
				break; 
			case "Save Cover Letter":
			case "Submit File":
			case "Edit":
				include( "coverletters/admin/edit.inc" );
				break;
			case "Cancel":
			case "delete":
				include( "coverletters/admin/delete_main.inc" );
				break;
			case "Delete":
				include( "coverletters/admin/delete.inc" );
				break;
// not doing applications here!!!
/*			case "activate":
				include( "coverletters/admin/activate_main.inc" );
				break;
			case "Activate":
				include( "coverletters/admin/activate.inc" );
				break;
			case "email":
				include( "coverletters/admin/email_employer.inc");
				break;
			case "application":
				include( "coverletters/admin/view_applications_main.inc" );
				break;
			case "View Applications":
			case "View All Applications":
			case "View Selected Applications":
				include( "coverletters/admin/view_applications.inc" );
				break;
			case "Search Applicants":
				include( "coverletters/admin/search_application_main.inc" );
				break;
			case "Search":
				include( "coverletters/admin/search_application.inc" );
				break;*/
			case view:
			default:
				include( "coverletters/admin/view_main.inc" );
		}
//		if ($page == "application" || $page == "View Applications" || $page=="View All Applications" || $page == "View Selected Applications" || $page== "Search Applicants" || $page=="activate" || $page == "Activate"){
//			include("coverletters/coverletter_footer.inc");
//		}else{
			include("coverletters/admin/footer.inc");
//		}
	}
}else{
	include ("welcome.inc");
}
?>
