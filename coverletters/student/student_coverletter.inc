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
 | Filename: student_coverletter.inc                                            |
 +------------------------------------------------------------------------------+
 | Description: This seems to be the main flow control page for student         |
 | coverletters.                                                                |
 +------------------------------------------------------------------------------+

*/
if ($userlevel == STUDENT && !empty($coverletter_id)){
    // Check to make sure that students can only modify their own stuff.
    $check_sql = "SELECT 1
                  FROM coverletter
                  WHERE coverletter_id = '".addslashes($coverletter_id)."'
                  AND student_number = '".addslashes($student_number)."'";
                  
    $check_result = $GLOBALS['dbh']->GetOne($check_sql);
    if ($check_result != 1){
        $page = '';
        if (!$PDF){
            $error =("You do not have access to that coverletter");
        }
    }
}


if ($PDF) {

	switch ($page) {

		case "View HTML":
			include ("coverletters/student/view_html.inc");
			break;

		case "View PDF":
			include ("PDF/PDFconversion/ConvertToPDF.class");
			include ("coverletters/student/view_pdf.inc");
			break;

		case "save":
		case "Submit File":
			if ($userlevel == ADMINSTUDENT){
				$save_error = "You cannot modify a student's resume from within here.";
				unset($PDF);
				break;	
			}
			include( "coverletters/save.inc" );
			if ($save_error == ""){
				include( "coverletters/student/view_html.inc" ); 
			} else {
				unset($PDF);
			}
			break;

		default:
			unset( $PDF );
	}
} else {

	include ('coverletters/coverletter_header.inc');
	if ($error) error($error);
    switch ($page) {

		// although this is still here, it's never referenced, because 'view' below does the same thing
		case 'edit':
		case 'Edit';
			include ('coverletters/student/edit_main.inc');
			break;

		case 'add':
		case 'Add':
			include ('coverletters/student/add_main.inc');
			break;

		case 'Add Template':
		case 'Add Generic':
		case 'Add Job':
			include ('coverletters/student/add_letter.inc');
			break;

/* we don't allow students to delete coverletters because of difficulty with applications
		case "delete":
			include( "coverletters/student/delete_main.inc" );
			break;

		case "Delete":
			if ($userlevel == ADMINSTUDENT){
				error("You cannot delete cover letters from here.");
				include("coverletters/student/delete_main.inc");
				break;
			}
			include( "coverletters/student/delete.inc" );
			break;
*/

		case 'Mamook':
		case 'view':
		default:
			include ('coverletters/student/view.inc');
			break;
	}

	include ('coverletters/coverletter_footer.inc');
}
?>
