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
 | Filename: view_pdf.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description: Displays the coverletter in PDF format to the user.             |
 +------------------------------------------------------------------------------+

*/

$error = 0;

if ($coverletter_id)
{
	$result = $GLOBALS['dbh']->Execute( "SELECT DISTINCT letter, student_number
		FROM coverletter
		WHERE coverletter_id = '" . addslashes( $coverletter_id ) . "'");
	if ($result->RecordCount() <= 0){
		$error = 1;
	}else{
		$row = $result->FetchRow();
		$student_num = $row["student_number"];
;
		$covert_class = new ConvertToPDF();
		$covert_class->convert_coverletter($coverletter_id, $student_num);
	}
    if ($error == 1){
        unset( $PDF );

        if ($type_id == GENERIC){
            $job_code = "Generic";
        }elseif($type_id == TEMPLATE){
            $job_code = "Template";
        }else{
            $result = $GLOBALS['dbh']->Execute("SELECT job_code
                        FROM job_info 
                        WHERE job_id = '" . $job_id . "'");
            $row = $result->FetchRow();
        }
        $error = ( "No cover letter exists for job code - " . $row["job_code"] . " - and student - " . $student_num . ".");
    }
} else {
	unset( $PDF );
    $error = ("Please choose a specific cover letter.");
    if ($student_num_passthru)
    {
        $student_num = $student_num_passthru;
        $page="View Cover Letters";
    }
}
?>

