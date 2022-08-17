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
 | Filename: history_summary_report.inc                                         |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/
 

/* Filename:  history_summary_report.inc
   Purpose:  Generates a report summarizing a set of history records in a PDF report

   Variables coming in from the previous page, view_history_results.inc:
    $history_id_list:  string holding comma-delimited history id's returned from advanced search by user

   Notes:
     apostrophe delimited strings are used frequently here to enclose proper html tags with double-quoted attribute values
*/
 
echo("</td><td align='left'><img src='misc/images". $_SESSION['SESSION_colorscheme'] ."/topbar_right.jpg' alt='' /></td></tr></table>");

$history_id_list = urldecode($history_id_list);
$history_id_array = explode(",", $history_id_list);
$history_id_array = array_unique($history_id_array);
$history_id_list = implode(",", $history_id_array);

if (!$history_id_list || trim($history_id_list) == '')
{
    error("No history records selected for this report.");
    return;
}
// fetch everything we need from the history, student and history_notes tables
$sql = "SELECT h.history_id
            , h.student_number, CONCAT(h.salary,'/',sp.salary_period_name) AS salary
            , h.student_work_phone, h.work_term_home_phone, h.work_email
            , h.work_term_street_address_1, h.work_term_street_address_2, h.work_term_city, h.work_term_postal_code
            , h.site_visit_date, h.site_visit_date_supervisor
            , h.company_name, h.department_name, h.company_street_address_1, h.company_street_address_2, h.company_city, h.company_postal_code
            , h.supervisor_name, h.supervisor_phone_number, h.supervisor_cellphone_number, h.supervisor_fax_number, h.supervisor_email, h.supervisor_position
            , h.supervisor_email
            , CONCAT(s.first_name, ' ', s.last_name) AS student_name
            , s.city_current, s.postal_code_current
            , s.phone_current
            , s.photo
            , s.email
            , sd.record_id
            , s.street_address_current, s.street_address_current2, s.street_address_current3
            , s_pl.abbreviation AS s_abbreviation
            , s_cl.country_name AS s_country_name
            , wt_pl.abbreviation AS wt_abbreviation
            , c_pl.abbreviation AS c_abbreviation
            , wt_cl.country_name AS wt_country_name
            , c_cl.country_name AS c_country_name
            , IF (hc.workterm_code, hc.workterm_code, h.work_term_number) AS work_term_number
        FROM history h
        INNER JOIN student_department sd
          ON h.student_number = sd.student_number AND h.department_id = sd.department_id
        LEFT JOIN student s
          ON h.student_number = s.student_number
        LEFT JOIN country_list s_cl
          ON s.country_current = s_cl.country_id
        LEFT JOIN provstate_list s_pl
          ON s.province_current = s_pl.provstate_id
        LEFT JOIN provstate_list wt_pl
          ON h.work_term_province = wt_pl.provstate_id
        LEFT JOIN provstate_list c_pl
          ON h.company_province = c_pl.provstate_id
        LEFT JOIN country_list wt_cl
          ON h.work_term_country = wt_cl.country_id
        LEFT JOIN country_list c_cl
          ON h.company_country = c_cl.country_id
        LEFT JOIN salary_period sp
          ON h.salary_period_id = sp.salary_period_id
        LEFT JOIN history_container hc
          ON h.history_id = hc.history_id
        WHERE h.history_id IN (".$history_id_list.")
        GROUP BY h.history_id
        ORDER BY s.last_name, s.first_name, work_term_number DESC, h.company_name, h.department_name";

// DEBUG
//echo ($sql."<br />");
$result = $GLOBALS['dbh']->Execute($sql);

// htmldoc commands
$export_string = '<!-- MEDIA LEFT 21mm
                       MEDIA RIGHT 21mm
                       MEDIA BOTTOM 24mm
                       MEDIA TOP 24mm
                       FOOTER LEFT "Report generated by '.BRAND_NAME.' on $DATE"
                       FOOTER RIGHT "$PAGE of $PAGES" -->';

$export_string .= '<h3>Placement Summary Report</h3>';

// begin building a big table to hold the report
$export_string .= '<table cellspacing="0" cellpadding="2" border="1" width="100%">';

$export_string .= '<tr>';
// make three empty columns that span for the entire row for the user's personal use
$export_string .= '<th class="rowgrey" width="5">1</th>';
$export_string .= '<th class="rowgrey" width="5">2</th>';
$export_string .= '<th class="rowgrey" width="5">3</th>';

$export_string .= '<th class="rowgrey">&nbsp;</th>';
$export_string .= '<th class="rowgrey" align="center"><b class="white">Student</b></th>';
$export_string .= '<th class="rowgrey" align="center"><b class="white">Work Location</b></th>';
$export_string .= '<th class="rowgrey" align="center"><b class="white">Supervisor</b></th>';
$export_string .= '<th class="rowgrey">&nbsp;</th>';
$export_string .= '</tr>';

for ($i = 0; $row = $result->FetchRow(); $i++)
{
    // htmldoc command to set the minimum number of lines needed before we break to a new page
    $export_string .= '<!-- NEED 9 -->';

    $export_string .= '<tr class="'.(($i % 2)?"row1d":"row0d").'">';

    // empties
    $export_string .= '<td>&nbsp;</td>';
    $export_string .= '<td>&nbsp;</td>';
    $export_string .= '<td>&nbsp;</td>';

    // student number, wt #, salary, work site visit date, work site visit supervisor
    $export_string .= '<td valign="top">';
    $export_string .= $row['student_number'].'<br />';
    $export_string .= 'WT# '.$row['work_term_number'].'<br />';
    $export_string .= ($row['salary'] ? '$'.$row['salary'].'<br />' : '');
    $export_string .= ($row['site_visit_date'] ? 'WSVstu:<br />'.$row['site_visit_date'].'<br />' : '');
    $export_string .= ($row['site_visit_date_supervisor'] ? 'WSVsup:<br />'.$row['site_visit_date_supervisor'] : '');
    $export_string .= '</td>';

    // student name, address, email, current and work phone numbers
    $export_string .= '<td valign="top">';
    $export_string .= $row['student_name'].'<br />';
    // crawl upwards to find the most relevant student address
    if ($row['work_term_street_address_1'] == null)
    {
        $address = ($row['street_address_current'] ? $row['street_address_current'].'<br />' : '');
        $address .= ($row['street_address_current2'] ? $row['street_address_current2'].'<br />' : '');
        $address .= ($row['street_address_current3'] ? $row['street_address_current3'].'<br />' : '');
        $address .= ($row['city_current'] ? $row['city_current'].', ' : '');
        $address .= ($row['s_abbreviation'] ? $row['s_abbreviation'] : '');
        $address .= ($row['postal_code_current'] ? '&nbsp;&nbsp;'.$row['postal_code_current'] : '');
        $address .= ($row['s_country_name'] ? '<br />'.$row['s_country_name'] : '');
    }
    else
    {
        $address = ($row['work_term_street_address_1'] ? $row['work_term_street_address_1'].'<br />' : '');
        $address .= ($row['work_term_street_address_2'] ? $row['work_term_street_address_2'].'<br />' : '');
        $address .= ($row['work_term_city'] ? $row['work_term_city'].', ' : '');
        $address .= ($row['wt_abbreviation'] ? $row['wt_abbreviation'] : '');
        $address .= ($row['work_term_postal_code'] ? '&nbsp;&nbsp;'.$row['work_term_postal_code'] : '');
        $address .= ($row['wt_country_name'] ? '<br />'.$row['wt_country_name'] : '');
    }
    $export_string .= $address.'<br />';
    // current phone number
    $export_string .= 'Home: '.($row['work_term_home_phone'] ? $row['work_term_home_phone'] : $row['phone_current']).'<br />';
    // work phone number
    $export_string .= 'Work: '.$row['student_work_phone'];
    $export_string .= ($row['email'] ? '<br />E-mail:  '.$row['email'] : '');
    $export_string .= ($row['work_email'] ? '<br />Work E-mail:  '.$row['work_email'] : '');
    $export_string .= '</td>';

    // company name, division, company address
    $export_string .= '<td valign="top">';
    $export_string .= $row['company_name'];
    $export_string .= ($row['company_name'] != $row['department_name'] ? ',<br />'.$row['department_name'].'<br />' : '<br />');
    // build the company address
    $address = ($row['company_street_address_1'] ? $row['company_street_address_1'].'<br />' : '');
    $address .= ($row['company_street_address_2'] ? $row['company_street_address_2'].'<br />' : '');
    $address .= ($row['company_city'] ? $row['company_city'].', ' : '');
    $address .= ($row['c_abbreviation'] ? $row['c_abbreviation'] : '');
    $address .= ($row['company_postal_code'] ? '&nbsp;&nbsp;'.$row['company_postal_code'] : '');
    $address .= ($row['c_country_name'] ? '<br />'.$row['c_country_name'] : '');
    $export_string .= $address;
    $export_string .= '</td>';

    $export_string .= '<td valign="top">';
    $export_string .= ($row['supervisor_name'] ? $row['supervisor_name']."<br />" : "&nbsp;");
    $export_string .= ($row['supervisor_position'] ? $row['supervisor_position'].'<br />' : '');
    $export_string .= ($row['supervisor_phone_number'] ? 'Ph:  '.$row['supervisor_phone_number'].'<br />' : '');
    $export_string .= ($row['supervisor_cellphone_number'] ? 'Cell:  '.$row['supervisor_cellphone_number'].'<br />' : '');
    $export_string .= ($row['supervisor_fax_number'] ? 'Fax:  '.$row['supervisor_fax_number'].'<br />' : '');
    $export_string .= ($row['supervisor_email'] ? 'E-mail:  '.$row['supervisor_email'].'<br />' : '');
    // get the history notes for this record
    $sql = "SELECT hn.notes
            FROM history_notes hn
            WHERE hn.history_id = ".$row['history_id'];
    $notes_result = $GLOBALS['dbh']->Execute($sql);
    while ($notes_row = $notes_result->FetchRow()) {
        $notes .= $notes_row['notes'].'<br />';
    }
    $export_string .= '</td>';

    // student's photo
    if ($row['photo'] == null) {
        $export_string .= '<td bgcolor="grey">&nbsp;</td>';
    }
    else {
        // determine the dimensions of the image.  if they're too big, set the img tag to scale it to prevent overflowing from the right side of the page
        $photo = PHOTO_DIR.$row['photo'];        // doesn't work because some people don't have the photo_path set?  fix later
        $num = rand(0,100);
        //$photo_uri = STUDENT_PHOTO_VIEWER_PATH."student_photo_viewer.php?record_id=".$row['record_id']."&amp;num=".$num;
        $photo_uri = $row['photo'];
        $image_resource = imagecreatefromjpeg($photo);
        $width = imagesx($image_resource);
        $height = imagesy($image_resource);

        // DEBUG
        //echo ($photo."<br />");

        if ($width >= 150) {
            $img_tag = '<img src="'.$photo_uri.'" width="'.($width / 2).'" height="'.($height / 2).'" alt="'.$row['student_name'].'" alt="" />';
        }
        else {
            $img_tag = '<img src="'.$photo_uri.'" alt="'.$row['student_name'].'" alt="" />';
        }

        $export_string .= '<td valign="center" align="center">'.$img_tag.'</td>';
    }

    $export_string .= '</tr>';
}

$export_string .= '</table>';

// create the object and make it create the html file containing our export_string
include("PDF/PDFconversion/ConvertToPDF.class");
$convert = new ConvertToPDF();
$convert->create_input_file($export_string);

// Print some messages to the screen and give them the export to PDF button
echo('<h3>Placement History - Placement Summary Report</h3>');
    echo('<table cellspacing="0" cellpadding="2" border="0">');
        echo('<tr class="row1">');
            echo('<td>');
            echo(sizeof($history_id_array).(sizeof($history_id_array) == 1 ? ' placement' : ' placements').' returned.  Report ready.<br />');
            echo('</td>');
        echo('</tr>');
        echo('<tr class="row1">');
            echo('<td>');
                echo('<hr />');
            echo('</td>');
        echo('</tr>');
        echo('<tr class="row1">');
            echo('<td align="center">');
                echo('<form action="'.$PHP_SELF.'" method=post name="myform">');
                    echo('<input type="submit" name="viewpdf" value="Export to PDF" /></td></tr>');
                    echo('<input type="hidden" name="select" value="reportOutput" />');
                    echo('<input type="hidden" name="convertObject" value="' . packObject($convert).'" />');
                    echo('<input type="hidden" name="PDF" value="1" />');
                echo('</form>');
            echo('</td>');
        echo('</tr>');
echo('</table>');

// DEBUG
//echo $export_string;

?>
