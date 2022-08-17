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
 | Filename: apply_student.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: Deals with student's applications, distributing to other        |
 | files whether viewing or deleting.                                           |
 +------------------------------------------------------------------------------+

*/


// see if we're viewing PDF
if ($PDF) {

$student[] = $student_number;

	include('PDF/PDFconversion/ConvertToPDF.class');
	$application = new ConvertToPDF();
	$application->temp_process_selected_applications($job_id, $student, 'yes');
}

// editing or deleting
elseif ($option == 'edit_delete') {

    if ($edit) {

        echo ("<h3>Job Applications - Edit</h3>");
        include ('applications/student/edit.inc');
    }

    elseif ($delete) {

        echo ("<h3>Job Applications - Delete</h3>");
        include ('applications/student/confirm_delete.inc');
    }

    else {

        if ($edit_save) {

            $sql = "UPDATE applications
                    SET transcript = '$include_transcript',
                    resume_id = '$resume_id'
                    WHERE application_id = '$application_id'
                    AND student_number = '$student_number'";
            if ($GLOBALS['dbh']->Execute ($sql)) {

                notify ("Changes to your application have been successfully saved.");
            }

            else {

                error ("There was an error saving your changes to your application.");
            }
        }

        elseif ($delete_confirm) {

            // get coverletter_id in order to delete it
            $sql = "SELECT coverletter_id
                    FROM applications
                    WHERE application_id = '$application_id'
                    AND student_number = '$student_number'";
            $result = $GLOBALS['dbh']->Execute ($sql);
            if ($row = $result->FetchRow()) {

                $sql = "DELETE FROM coverletter
                        WHERE coverletter_id = '". $row["coverletter_id"] ."'
                        AND student_number = '$student_number'";
                $GLOBALS['dbh']->Execute ($sql);
            }

            $sql = "DELETE FROM applications
                    WHERE application_id = '$application_id'
                    AND student_number = '$student_number'";
            if ($GLOBALS['dbh']->Execute ($sql)) {

                notify ("Your application has been successfully deleted.");
            }

            else {

                error ("There was an error while attempting to delete your application.");
            }
        }

        include ("applications/student/edit_delete.inc");
    }
}

else {

	switch ($option) {

		case "add_special":
			include ("applications/student/add_special.inc");
			break;

		case "add":
		case "add_online":
			include ("applications/student/add.inc");
			break;

		case "view":
			include ("applications/student/view_apps_list.inc");
			break;

		case "edit_delete":
            include ("applications/student/edit_delete.inc");
			break;

		default:
			include ("applications/student/view_apps_list.inc");
			break;
	}

} // else not PDF
