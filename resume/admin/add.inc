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
 | Filename: add.inc                                                            |
 +------------------------------------------------------------------------------+
 | Description: Ensures that all info entered in add_main.inc is valid and      |
 | then includes edit page to edit new resume                                   |
 +------------------------------------------------------------------------------+

*/

//ensure correct student number/name
list($student_num, $error) = find_student($student_name_select, $student_number_select, $student_num);

if ( $student_num == -1 ){
	if ($error == "duplicate"){
		//more than one student with selected anem
		include( "misc/names.inc" );
		list($first, $last) = explode(" ", $student_name_select);
		select_name($first, $last, $PHP_SELF . "&amp;select=resume&amp;PDF=1&amp;page=" . urlencode($page) . "&amp;department_id=" . urlencode($department_id) . "&amp;name=" . urlencode($name));
	}else{
		error( $error );
		include( "resume/admin/add_main.inc" );
	}
}else{
	if (trim($name) == ""){
		error ("Invalid Resume Name.");
		include( "resume/admin/add_main.inc" );
	}else{
		$type_id = SPECIFIC;
		$resume_id = "";
		$last_modified = "";
		include( "resume/admin/edit.inc" );
	}
}
?>
