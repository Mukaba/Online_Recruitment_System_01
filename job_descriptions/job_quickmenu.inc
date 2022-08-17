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
 | Filename: job_quickmenu.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: Quickmenu for job description edit/view                         |
 |                                                                              |
 | Variable that needs to come in to this form: $job_id                         |
 +------------------------------------------------------------------------------+

*/

$sql = "SELECT ji.contact_id, ji.employer_department_id, ji.employer_id, i.job_id 
        FROM job_info AS ji
        LEFT OUTER JOIN interview AS i
        ON ji.job_id=i.job_id
        WHERE ji.job_id='".$job_id."'";
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();


$html .= '<td align="right">';
    $html .= '<form action="'.$PHP_SELF.'" method="post">';
        $html .= '<input type="hidden" name="contact_id" value="'.$row['contact_id'].'" />';
        $html .= '<input type="hidden" name="department_id" value="'.$row['employer_department_id'].'" />';
        $html .= '<input type="hidden" name="employer_id" value="'.$row['employer_id'].'" />';
        $html .= '<input type="hidden" name="job_code" value="'.$row['job_code'].'" />';
        $html .= '<input type="hidden" name="job_id" value="'.$job_id.'" />';
        $html .= '<input type="hidden" name="continue" value="view_applicants" />';
        $html .= '<input type="hidden" name="option" value="view" />';
        $html .= '<input type="hidden" name="PDF" value="1" />';
        $html .= '<input type="hidden" name="show_quick" value="true" />';

// html for the menu.  use failsafes to remove options in case we're viewing a
// fubar'ed record without employer_id's, department_id's, contact_id's and whatnot
        $html .= '<select name="select">';
            $html .= '<option value="view_job">View this Job</option>';
            $html .= '<option value="edit_job">Edit this Job</option>';
            $html .= ($row['employer_id'] ? '<option value="view_this_company">View this Job\'s Company</option>' : '');
            $html .= ($row['employer_department_id'] ? '<option value="view_this_division">View this Job\'s Division</option>' : '');
            $html .= ($row['contact_id'] ? '<option value="view_this_contact">View this Job\'s Contact</option>' : '');
            $html .= ($row['job_id'] ? '<option value="view_this_interview">View this Job\'s Interviews</option>' : ''); 
            $html .= '<option value="applications">View Applications</option>';
            $html .= '<option value="set_actions">View/Set Contact Actions</option>';  
        $html .= '</select>';
        $html .= '&nbsp;<input type="image" src="misc/images/arrowg.gif" alt="go" />';
    $html .= '</form>';
$html .= '</td>';

echo $html;
?>
