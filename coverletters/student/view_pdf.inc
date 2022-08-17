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
 | Description: Displays the user their selected coverletter in PDF format.     |
 +------------------------------------------------------------------------------+

*/

//if ($job_position != ""){
//        $job_id = $job_id[$job_position];
//        $type_id = $type_id[$job_position];
//	if ($job_id == ""){
//		$job_id = "0";
//	}
//}

if (!is_array($job_id) && $coverletter_id){// && $job_id != ""){
	$covert_class = new ConvertToPDF();
    
	$covert_class->convert_coverletter($coverletter_id, $student_number);
}else{
	include( "header.inc" );
	include( "coverletters/coverletter_header.inc" );
	error( "Please select a job." );
	include( "coverletters/student/view.inc" );
	include( "coverletters/coverletter_footer.inc" );
	include( "footer.inc" );
}
?>

