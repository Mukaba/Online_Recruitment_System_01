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
 | Filename: main.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/
 
// Filename:  main.inc
// Description:  form that gets a time period from the user and sends it off to contacts_pdf_batch.inc to build a report

// close the stuff already prepended to each page
$html = '</td><td align="left"><img src="misc/images'.$_SESSION['SESSION_colorscheme'].'/topbar_right.jpg" alt="" /></td></tr></table>';

$html .= '<h3 align="left">Employer Information - Job Development Report</h3>';
$html .= '<form method="post" name="myform" action="'.$PHP_SELF.'&amp;select=job_development_report">';
    $html .= '<input type="hidden" name="contact_id_string" value="'.$contact_id_string.'" />';
        $html .= '<table class="row1" cellpadding="5" cellspacing="0" border="0">';
        $html .= '<tr><td colspan="2" align="left" class="tableheaders">Set a time period for placements made by each contact:</td></tr>';
        $html .= '<tr>';
            $html .= '<td align="right">From: <b class="red">*</b></td>';
            $html .= '<td><input type="text" name="min_date" maxlength="40" size="20" />&nbsp;'.popup('min_date', 'myform').'</td>';
        $html .= '</tr>';

        $html .= '<tr>';
            $html .= '<td align="right">To: </td>';
            $html .= '<td><input type="text" name="max_date" maxlength="40" size="20" />&nbsp;'.popup('max_date', 'myform');
                $html .= '<br /><font size="2">(defaults to current date if not specified)</font></td>';
        $html .= '</tr>';
        
        /*
        $html .= '<tr>';
            $html .=  '<td colspan=2>&nbsp;</td>';
        $html .= '</tr>';
        */

        $html .= '<tr>';
            $html .= '<td align="right">Maximum number <br />of job placements to show: </td>';
            $html .= '<td><input type="text" name="show_placements" maxlength="15" size="5" />';
                $html .= '<br /><font size="2">(defaults to show all)</font></td>';
        $html .= '</tr>';

        // get all the departments and construct an array to use in generating a pulldown menu
        $sql = 'SELECT department_id, department_name
                FROM department
                WHERE using_full_system = 1
                ORDER BY department_name';
        $result = $GLOBALS['dbh']->Execute($sql);
        
        $html .= '<tr>';
            $html .= '<td align="right">Show placements from: </td>';
            $html .= '<td><select name="department">';

            // build a string to hold the option values of the pulldown menu
            while ($row = $result->FetchRow()) {
                $option_string .= '<option value="'.$row['department_id'].'"'.$selected.'>'.$row['department_name'].'</option>';
                $department_ids[] = $row['department_id'];
            }
            // prepend an all departments option to the string, make it pre-selected
            $department_ids = implode(',', $department_ids);
            $option_string  = '<option value="'.$department_ids.'" selected="selected">All Departments</option>'.$option_string;
            
            $html .= $option_string;
            $html .= '</select></td>';
        $html .= '</tr>';

        // ordering
        $html .= '<tr>';
            $html .= '<td align="right">Order report by: </td>';
            $html .= '<td><select name="order">';
                $html .= '<option value="contact_name" selected="selected">Contact Name</option>';
                $html .= '<option value="company_name">Company Name</option>';
            $html .= '</select></td>';
        $html .= '</tr>';

        // whether or not to include contact notes
        $html .='<tr>';
            $html .= '<td align="right"><input type="checkbox" name="show_contact_notes" value="true" /></td>';
            $html .= '<td>Show contact notes</td>';
        $html .= '</tr>';

        $html .= '<tr>';
            $html .= '<td colspan="2"><hr /></td>';
        $html .= '</tr>';

        $html .= '<tr>';
            $html .= '<td colspan="2" align="center"><input type="submit" value="Continue" /></td>';
        $html .= '</tr>';

    $html .=  '</table>';
$html .=  '</form>';

echo $html;
?>
