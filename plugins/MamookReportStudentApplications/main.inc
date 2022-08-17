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
// Description:  form to set some options for the report 

// close the stuff already prepended to each page
$html = '</td><td align="left"><img src="misc/images'.$_SESSION['SESSION_colorscheme'].'/topbar_right.jpg" alt="" /></td></tr></table>';
$html .= '<h3 align="left">Student Information - Student Applications Report</h3>';
$html .= '<form method="post" name="myform" action="'.$PHP_SELF.'&amp;select=student_applications_report">';
    $html .= '<input type="hidden" name="record_id_list" value="'.$record_id_list.'" />';
        $html .= '<table class="row1" cellpadding="7" cellspacing="0" border="0" >';
        $html .= '<tr><td align="left" class="tableheaders">Choose a set of terms: </td></tr>';
        // term selection
        $html .= '<tr><td align="center"><table class="row0" cellpadding="3" cellspacing="0" border="0" >';

    // get the terms in the database
        $sql = 'SELECT term_id, term_name
                FROM term
                ORDER BY order_by';
        $results = $GLOBALS['dbh']->Execute($sql);
        while ($row = $results->FetchRow()) {
            $terms[$row['term_id']] = $row['term_name'];
        }
        $i = $limit = $results->RecordCount();

        // output the choices to the user 
        $html .= '<tr>';
        for ($j = 1; $j <= $limit; $j++) {
            $html .= '<td>';
            $html .= '<input type="checkbox" name="termsused['.$j.']" value="'.$j.'" />';
            $html .= '<select name="term['.$j.']">';
            foreach ($terms as $term_id=>$term_name) {
                if ($term_id == $i) {
                    $html .= '<option value="'.$term_id.'" selected="selected">'.$term_name.'</option>';
                } else {
                    $html .= '<option value="'.$term_id.'">'.$term_name.'</option>';
                }
            }//end foreach
            $html .= '</select>';

            // choice of years
            $results = $GLOBALS['dbh']->Execute('SELECT DISTINCT year FROM history ORDER BY year DESC');
            $today   = getdate();
            $html .= '<select name="year['.$j.']">';
            while ($row = $results->FetchRow()) {   
                if ($today['year'] == $row['year']) {   
                    $html .= '<option value="'.$row['year'].'" selected="selected">'.$row['year'].'</option>';
                } else {   
                    $html .= '<option value="'.$row['year'].'">'.$row['year'].'</option>';
                }
            }//end while
            $html .= '</select>';
            $html .= '</td>';

            if ($j < $limit) {   
               $html .= '<tr>';
            }

            switch ($i) {   
            case $limit:
                $i = 1;
                break;
            default:
                $i++;
                break;
            }
        }//end of for
        $html .= '</tr>';
        $html .='</table></td></tr>';
       
        
        
            $html .= '<tr><td align="left" class="tableheaders">Order the report using:</td></tr>';
            // ordering
            $html .= '<tr><td align="center"><table class="row0" cellpadding="3" cellspacing="0" border="0">';
            $html .= '<tr>';
                $html .= '<td>Order 1: </td>';
                $html .= '<td><select name="order_by_1">';
                    //$html .= '<option value=""></option>';
                    $html .= '<option value="advisor">Coop Advisor</option>';
                    $html .= '<option value="discipline">Discipline</option>';
                    $html .= '<option value="apps_count">Number of Applications</option>';
                    $html .= '<option value="interview_count">Number of Interviews</option>';
                    $html .= '<option value="last_name" selected="selected">Student Last Name</option>';
                    $html .= '<option value="work_term_number">Work Term Number</option>';
                $html .= '</select></td>';
            $html .= '</tr>';

            $html .= '<tr>';
                $html .= '<td>Order 2: </td>';
                $html .= '<td><select name="order_by_2">';
                    //$html .= '<option value=""></option>';
                    $html .= '<option value="advisor">Coop Advisor</option>';
                    $html .= '<option value="discipline">Discipline</option>';
                    $html .= '<option value="apps_count">Number of Applications</option>';
                    $html .= '<option value="interview_count">Number of Interviews</option>';
                    $html .= '<option value="last_name">Student Last Name</option>';
                    $html .= '<option value="work_term_number">Work Term Number</option>';
                $html .= '</select></td>';
            $html .= '</tr>';
        $html .= '</table></td></tr>';
        // print("</td></tr>'");

        // whether or not to include job numbers and company names?
        $html .='<tr>';
            $html .= '<td><input type="checkbox" name="show_applications" value="true" checked="checked" /> Show list of applications</td>';
        $html .= '</tr>';

        $html .= '<tr>';
            $html .= '<td><hr /></td>';
        $html .= '</tr>';

        $html .= '<tr>';
            $html .= '<td align="center"><input type="submit" value="Continue" /></td>';
        $html .= '</tr>';
        
$html .=  '</table>';
$html .=  '</form>';
//$html .=  '</table>';
echo $html;
?>
