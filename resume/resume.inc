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
 | Filename: resume.inc                                                         |
 +------------------------------------------------------------------------------+
 | Description: This file selects with files to include based on the            |
 | userlevel of the user and the $page variable. If the $PDF variable is set    |
 | and an error occurs the $error variable gets set and the $PDF variable gets  |
 | unset. This file will then be called again (see mamook.php) and the error   |
 | message will be displayed.                                                   |
 +------------------------------------------------------------------------------+

*/

// Let the employer view a resume.

if ($userlevel == EMPLOYER) {

	if ($PDF) {
	
		switch ($page) {
		
			case "View PDF":
				include ( "PDF/PDFconversion/ConvertToPDF.class" );
				include("resume/admin/view_pdf.inc");
				break;
			
			default:
				include ("welcome.inc");
				break;
		}
	}
}
elseif ($userlevel == OFFICE || $userlevel == FACULTY) {
	if ($department_id == "") {
		if ($departments_in_group > 1) {
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
	        while($row=$result_group->FetchRow()) {
	            $pulldown_menu_group=$row['pulldown_menu_group'];
        	}
	                
			if ($pulldown_menu_group=='1') {
                $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
                $department_id='0';
	        } else {
	            $department_id = $auth->department;
	        }
	    } else {
	        $department_id = $auth->department;
	    }
	}

	if ($department_id != "0" && !in_array($department_id, $departments_in_group)) {
	
		$error = ("You do not have access to this department");
		$department_id = $auth->department;
		$page = "";
		unset( $PDF );
	}
	
	if ($department_id == "0") {
		$department_in_str = "'" . implode("', '", $departments_in_group) . "'";
	} else {
		$department_in_str = "'" . $department_id . "'";
	}

	
	if ($PDF) {
	
		switch($page) {
		
			case "View HTML":
				include ("resume/admin/view_html.inc");
				break;
			case "View PDF":
				include ( "PDF/PDFconversion/ConvertToPDF.class" );
				include("resume/admin/view_pdf.inc");
				break;
			case "Update Resume":
			case "Submit File":
				include_once ("resume/save.inc");
				if ($save_error == ""){
					include("resume/admin/view_html.inc");
				}else{
					$error = $save_error;
					unset( $PDF );
				}
				break;
			default:
				unset( $PDF );
		}
	}else{
		include( "resume/admin/header.inc" );
		if ($error != ""){
			error($error);
			$error = "";
		}
		switch ($page){

			case "add":
		 		include( "resume/admin/add_main.inc" );
				break;
			 case "Add":
			 	include( "resume/admin/add.inc" );
				break;
			 
			 case "Mamook":
			 case "View Resumes":
				include( "resume/admin/view_all.inc" );
				break;
			 
			 case "view":
				include( "resume/admin/view_main.inc" );
				break;
	 
			 case "Edit":
				include( "resume/admin/edit.inc" );
			    	break;
			 
			 case "delete":
			 	include( "resume/admin/delete_main.inc" );
				break;
			 
			 case "View & Delete":
			 case "Delete":
			 	include( "resume/admin/delete.inc" );
				break;
			 default:
				include( "resume/admin/view_main.inc" );
				break;
		}
		include("resume/admin/footer.inc");
	 	
	}
}

elseif ($userlevel == STUDENT || $userlevel == ADMINSTUDENT) {

    if ($userlevel == ADMINSTUDENT) {

        $student_number = $student_number_login;
    }

	if ($PDF) {
		switch($page) {
			case "View PDF":
				include ( "PDF/PDFconversion/ConvertToPDF.class" );
				include ('resume/student/view_pdf.inc');
				break;
			case "View HTML":
				include ("resume/student/view_html.inc");
				break;
			case "Create HTML Resume":

				include ("resume/student/buildhtmlX.inc");
				break;
			case "Update Resume":
			case "Submit File":
			case "Save Resume":
				if ($userlevel == ADMINSTUDENT){
					$save_error = "You cannot modify a student's resume from within here.";
					unset($PDF);
				}else{
					include_once ("resume/save.inc");
					if ($save_error != ""){
						unset($PDF);
					}else{
						include("resume/student/view_html.inc");
					}
				}
				break;
			default:
				unset($PDF);
		}
	}
	
	else {
		include( "resume/resume_header.inc" );
		if ($error != "") {

			error($error);
			$error = "";
		}


		else {

			switch ($page) {

				case "Modify Format":
					include ("resume/student/bannerX.inc");
					break;

				case "delete":
					include( "resume/student/delete_main.inc" );
					break;

				case "Delete":
					if ($userlevel == ADMINSTUDENT) {

						error("<h1 class=\"black\">Error</h1><p>You are not allowed to delete a resume for a student.</p>");
								//include( "resume/student/delete_main.inc" );
					}

					else {
						include( "resume/student/delete.inc" );
					}
					break;
		
				case "view":
				case "Mamook":
					 include ("resume/student/view_main.inc");
					 break;

				case "add":
					include ("resume/student/add_main.inc" );
					break;
					
				case "Edit":
				case "edit":
					include("resume/student/edit_main.inc");
					break;

				case "Save Resume":
					if ($userlevel == ADMINSTUDENT){
						error("You cannot modify student's resumes from here.");
						include ("resume/student/view_main.inc"); 
					}else{
						include_once ( "resume/save.inc" );
						if ($save_error == "") {
							include ('resume/student/view_html.inc');
						}
						else {
							error ($save_error);
						}
					}
					break;
				case "Update Resume":
				case "Submit File":
					if ($save_error != ""){
						error($save_error);
						if ($userlevel == ADMINSTUDENT)
							include ("resume/student/view_main.inc");
					}else{
						include("resume/student/edit.inc");
					}    
					
					break;
	
				default:
					include( "resume/student/view_main.inc" );
					break;
			}
		}

		include("resume/resume_footer.inc");
	}
}else{
	include("welcome.inc");


}

?>
