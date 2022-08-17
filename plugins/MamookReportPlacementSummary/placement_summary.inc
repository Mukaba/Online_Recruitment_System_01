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
| Filename: placement_summary.php                                              |
+------------------------------------------------------------------------------+
| Description:                                                                 |
+------------------------------------------------------------------------------+

*/
?>
</td><td align="left" valign="top"><img src="misc/images<?php echo $_SESSION['SESSION_colorscheme']; ?>/topbar_right.jpg" alt="" /></td></tr></table>
<?php
$department_id=$auth->department;
$department_name=getDepartmentName($department_id);

if ($placement_submit == 'Go')
{
    $term_id = $term;
} else {
    $NextTermYear = getNextTermYear();
    $term_id=$NextTermYear['term'];
    $year=$NextTermYear['year'];
}

include('reports/PlacementCounter.class');
$pc = new PlacementCounter();

echo("<table border='0' cellspacing='0' cellpadding='2' width='99%'><tr><td>");
echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='placement_summary' />");
echo("<h3 align='left'>Placement Summary for ".$department_name."</h3>");
echo("<table cellpadding='5' cellspacing='0' border='0' class='row1'>");
echo("<tr><td><b class='black'>Term: </b>&nbsp;</td>");
echo("<td>");
echo("<select name='term'>");
$sqlterm = "SELECT term_id,term_name,year_order
    FROM term
    ORDER BY year_order";
$sqlterm_result = $GLOBALS['dbh']->Execute($sqlterm);
while ($sqltermrow = $sqlterm_result->FetchRow()) {
    if ($term_id == $sqltermrow['term_id']) {
        echo("<option value='".$sqltermrow['term_id']."' selected='selected'>".$sqltermrow['term_name']."</option>");
    } else {
        echo("<option value='".$sqltermrow['term_id']."'>".$sqltermrow['term_name']."</option>");
    }
}
echo("</select>");
echo("<select name='year'>");
$sqlyear = "
    SELECT MIN(year) AS year
    FROM history
    WHERE year != '0'
    AND department_id='".$department_id."'
    ";
$sqlyear_result = $GLOBALS['dbh']->Execute($sqlyear);
$year_row = $sqlyear_result->FetchRow();
$end_year = date("Y") + 2;

for ($i=$end_year;$i>=(int)$year_row['year'];$i--)
{
    $selected="";
    if ($year==$i){
        $selected="selected='selected'";
    }
    ?>
        <option <?php echo $selected?> value="<?php echo $i;?>"><?php echo $i;?></option>
        <?php
        $selected="";
}
echo("</select>");
echo("</td>");
echo("<td>");
echo("<input type='submit' name='placement_submit' value='Go' />");
echo("</td>");

echo("</tr></table>");

// This section contains all of the database queries used to determine the stats

// Students scheduled for a workterm
// including workterm challenges
$sql_scheduled = "
        SELECT count(DISTINCT sd.record_id) AS scheduled
        FROM student_department AS sd
        INNER JOIN semesters_table AS st ON st.record_id=sd.record_id
        LEFT JOIN semesters AS s ON s.semesters_id=st.semesters_id
        LEFT JOIN student_flags_join AS sfj ON sfj.record_id=sd.record_id
        WHERE s.description_type='1'
        AND st.term_id='".$term_id."' AND st.year='".$year."'
        AND sfj.student_flags_id = '".CURRENT_FLAG."'
        AND sd.department_id='".$department_id."'
        ";
$result_scheduled = $GLOBALS['dbh']->Execute($sql_scheduled);
$row_scheduled = $result_scheduled->FetchRow();
$scheduled = $row_scheduled['scheduled'];

// Students scheduled for a workterm by discipline
// -- this section was never added

// Students eligible for a workterm (unplaced)
$sql_unplaced = "SELECT count(ep.record_id) AS unplaced
        FROM eligible_placed as ep, student_department as sd
        WHERE ep.term_id='".$term_id."' AND ep.year='".$year."'
        AND ep.record_id = sd.record_id
        AND ep.student_flags_id='".ELIGIBLE_FLAG."'
        AND sd.department_id='".$department_id."'";
$result_unplaced = $GLOBALS['dbh']->Execute($sql_unplaced);
$row_unplaced = $result_unplaced->FetchRow();
$unplaced = $row_unplaced['unplaced'];

// Students placed
$sql_placed = "SELECT h.history_id
        , h.job_id
        , h.employer_id
        , h.start_date
        , h.end_date
        , h.work_term_hours_per_week
        , h.work_term_length
        , sd.discipline_id, d.discipline_name 
        , al.area_id, al.area_name
        , ec.company_type_id, ct.type_name
        , ed.industry_id, i.industry_name
        FROM history AS h
        LEFT JOIN employer_department AS ed ON ed.department_id=h.company_department_id
        INNER JOIN employer_company AS ec ON ec.employer_id=h.employer_id
        INNER JOIN student_department AS sd ON (sd.student_number=h.student_number AND sd.department_id=h.department_id)
        LEFT JOIN discipline AS d ON d.discipline_id=sd.discipline_id
        LEFT JOIN country_list AS cl ON cl.country_id=h.company_country
        LEFT JOIN area_list AS al ON al.area_id=cl.area_id
        LEFT JOIN company_type AS ct ON ct.type_id=ec.company_type_id
        LEFT JOIN industries AS i ON i.industry_id=ed.industry_id
        WHERE h.term_id='".$term_id."' AND h.year='".$year."'
        AND h.department_id='".$department_id."'";
$result_placed = $GLOBALS['dbh']->Execute($sql_placed);
$placed=0;
$placed_job=0;
$placed_own_and_return=0;
$placed_challenge=0;
$placed_transfer=0;
while ($row_placed = $result_placed->FetchRow())
{
    $discipline_id = $row_placed['discipline_id'];
    $start_date = $row_placed['start_date'];
    $end_date = $row_placed['end_date'];
    $work_term_hours_per_week = $row_placed['work_term_hours_per_week'];
    $work_term_length = $row_placed['work_term_length'];

    $increment = $pc->countPlacement($discipline_id, $start_date, $end_date, $work_term_length, $work_term_hours_per_week);

    $placed += $increment;
    if ($row_placed[job_id] != 0 && $row_placed[job_id] != NULL)
    {
        $placed_job += $increment;
    }
    else if ($row_placed[employer_id] == WT_CHALLENGE)
    {
        $placed_challenge += $increment;
    }
    else if ($row_placed[employer_id] == WT_TRANSFER)
    {
        $placed_transfer += $increment;
    }
    else
    {
        $placed_own_and_return += $increment;
    }

    // exclude transfer students
    if ($row_placed['employer_id'] != WT_TRANSFER) {
        if ($row_placed['discipline_id'] != 0)
        {
            $placed_discipline_list[$row_placed['discipline_id']]['disc_name'] = $row_placed['discipline_name'];
            $placed_discipline_list[$row_placed['discipline_id']]['count'] += $increment;
        }
        if ($row_placed['company_type_id'] != 0)
        {
            $placed_company_type_list[$row_placed['company_type_id']]['company_type_name'] = $row_placed['type_name'];
            $placed_company_type_list[$row_placed['company_type_id']]['count'] += $increment;
        }
        if ($row_placed['industry_id'] != 0)
        {
            $placed_industry_list[$row_placed['industry_id']]['industry_name'] = $row_placed['industry_name'];
            $placed_industry_list[$row_placed['industry_id']]['count'] += $increment;
        }
        if ($row_placed['area_id'] != 0)
        {
            $placed_area_list[$row_placed['area_id']]['area_name'] = $row_placed['area_name'];
            $placed_area_list[$row_placed['area_id']]['count'] += $increment;
        }
    }
}

// Students placed by region
$sql_placed_region = ("
    SELECT h.history_id
    , rl.region_id, rl.region_name, pl.abbreviation
    , h.start_date
    , h.end_date
    , h.work_term_hours_per_week
    , h.work_term_length
    , sd.discipline_id
    FROM history AS h
    INNER JOIN student_department sd
    ON h.student_number = sd.student_number AND h.department_id = sd.department_id
    LEFT JOIN region_list AS rl ON rl.region_id=h.company_region_id
    LEFT JOIN provstate_list AS pl ON pl.provstate_id=h.company_province
    WHERE h.term_id='".$term_id."' AND h.year='".$year."'
    AND h.employer_id <> '".WT_TRANSFER."'
    AND h.company_country = '". CANADA ."'
    AND h.department_id='".$department_id."'
    ");
$result_placed_region = $GLOBALS['dbh']->Execute($sql_placed_region);
$placed_canada = 0;
while ($row_placed_region = $result_placed_region->FetchRow())
{
    $discipline_id = $row_placed_region['discipline_id'];
    $start_date = $row_placed_region['start_date'];
    $end_date = $row_placed_region['end_date'];
    $work_term_hours_per_week = $row_placed_region['work_term_hours_per_week'];
    $work_term_length = $row_placed_region['work_term_length'];

    $increment = $pc->countPlacement($discipline_id, $start_date, $end_date, $work_term_length, $work_term_hours_per_week);
    $placed_canada += $increment;
    if ($row_placed_region['region_id'] != 0)
    {
        $prov_region = $row_placed_region['abbreviation']." - ".$row_placed_region['region_name'];
        $placed_region_list[$row_placed_region['region_id']]['region_name'] = $prov_region;
        $placed_region_list[$row_placed_region['region_id']]['count'] += $increment;
    }
}

// Students placed in return jobs
$sql_placed_return = ("
        SELECT DISTINCT h1.history_id
        , h1.start_date
        , h1.end_date
        , h1.work_term_hours_per_week
        , h1.work_term_length
        , sd.discipline_id
        FROM history AS h1
        INNER JOIN term AS t1
        ON h1.term_id=t1.term_id
        INNER JOIN term AS t2 
        ON h2.term_id=t2.term_id
        INNER JOIN history AS h2
        ON (h1.student_number=h2.student_number AND h2.company_department_id=h1.company_department_id)
        INNER JOIN student_department sd
        ON h1.student_number = sd.student_number AND h1.department_id = sd.department_id
        WHERE h1.term_id='".$term_id."' AND h1.year='".$year."'
        AND (h1.job_id IS NULL OR h1.job_id = '') 
        AND h1.employer_id != '".WT_CHALLENGE."'
        AND h1.employer_id != '".WT_TRANSFER."'
        AND h1.department_id='".$department_id."'
        AND h2.employer_id != '".WT_CHALLENGE."'
        AND h2.employer_id != '".WT_TRANSFER."'
        AND h2.department_id='".$department_id."'
        AND ((h2.year < h1.year) OR ((h2.year = h1.year) AND (t2.year_order < t1.year_order)))
        ");
$result_placed_return = $GLOBALS['dbh']->Execute($sql_placed_return);
while ($row_placed_return = $result_placed_return->FetchRow())
{
    $discipline_id = $row_placed_return['discipline_id'];
    $start_date = $row_placed_return['start_date'];
    $end_date = $row_placed_return['end_date'];
    $work_term_hours_per_week = $row_placed_return['work_term_hours_per_week'];
    $work_term_length = $row_placed_return['work_term_length'];

    $increment = $pc->countPlacement($discipline_id, $start_date, $end_date, $work_term_length, $work_term_hours_per_week);
    $placed_return += $increment;
}
//$placed_return = $row_placed_return['placed_return'];

// Students in own jobs, not counting those who return
$placed_own = $placed_own_and_return - $placed_return;

// The following section contains the HTML code to display the results of the report
echo("<table>");
echo("<tr>");
    echo("<td align='right'>$scheduled</td>");
    echo("<td>Students scheduled for a workterm (including challenges)</td>");
echo("</tr>");
echo("<tr>");
    echo("<td align='right'>".$unplaced."</td>");
    echo("<td>Students eligible for a workterm (unplaced students)</td>");
echo("</tr>");
echo("<tr>");
    echo("<td align='right'>".($placed - $placed_transfer)."</td>");
    echo("<td>Students placed + $placed_transfer Transfer".(($placed_transfer != 1) ? "s" : "")."</td>");
echo("</tr>");
echo("</table>");
echo("<table>");

// Type of placement
if ($placed_job || $placed_own || $placed_return || $placed_challenge || $placed_transfer)
{
    echo("<tr><td colspan='3'>&nbsp;</td></tr>");
    echo("<tr>");
        echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        echo("<td align='right'>&nbsp;</td>");
        echo("<td><b>Type of placement:</b></td>");
    echo("</tr>");
    echo("<tr>");
        echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        echo("<td align='right'>".$placed_job."</td>");
        echo("<td>Placed in posted job".(($placed_job > 1) ? "s" : "")."</td>");
    echo("</tr>");
    echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$placed_own."</td>");
        echo("<td>Placed in own job".(($placed_own > 1) ? "s" : "")."</td>");
    echo("</tr>");
    echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$placed_return."</td>");
        echo("<td>Placed in return job".(($placed_return > 1) ? "s" : "")."</td>");
    echo("</tr>");
    echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$placed_challenge."</td>");
        echo("<td>Work Term Challenge".(($placed_challenge > 1) ? "s" : "")."</td>");
    echo("</tr>");
    echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$placed_transfer."</td>");
        echo("<td>Work Term Transfer".(($placed_transfer > 1) ? "s" : "")."</td>");
    echo("</tr>");
    echo("<tr><td colspan='3'>&nbsp;</td></tr>");
}

//Placement by discipline 
$placed_discipline_count=0;
if ($placed_discipline_list)
{
    echo("<tr>");
        echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        echo("<td align='right'>&nbsp;</td>");
        echo("<td><b>Placement by discipline:</b></td>");
    echo("</tr>");
    asort($placed_discipline_list);
    reset($placed_discipline_list);
    foreach($placed_discipline_list as $arr)
    {
        $placed_discipline_count+=$arr['count'];
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$arr['count']."</td>");
        echo("<td>".$arr['disc_name']."</td>");
        echo("</tr>");
    }
    if ($placed - $placed_discipline_count > 0)
    {
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".($placed - $placed_discipline_count - $placed_transfer)."</td>");
        echo("<td>Unknown</td>");
        echo("</tr>");
    }

    echo("<tr><td colspan='3'>&nbsp;</td></tr>");
}

//Placement by company type
$placed_company_type_count=0;
if ($placed_company_type_list)
{
    echo("<tr>");
        echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        echo("<td align='right'>&nbsp;</td>");
        echo("<td><b>Placement by company type:</b></td>");
    echo("</tr>");
    asort($placed_company_type_list);
    reset($placed_company_type_list);
    foreach($placed_company_type_list as $arr)
    {
        $placed_company_type_count+=$arr['count'];
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$arr['count']."</td>");
        echo("<td>".$arr['company_type_name']."</td>");
        echo("</tr>");
    }
    if ($placed - $placed_company_type_count > 0)
    {
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".($placed - $placed_company_type_count - $placed_transfer)."</td>");
        echo("<td>Unknown</td>");
        echo("</tr>");
    }

    echo("<tr><td colspan='3'>&nbsp;</td></tr>");
}

//Placement by industry 
$placed_industry_count=0;
if ($placed_industry_list)
{
    echo("<tr>");
        echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        echo("<td align='right'>&nbsp;</td>");
        echo("<td><b>Placement by industry:</b></td>");
    echo("</tr>");
    asort($placed_industry_list);
    reset($placed_industry_list);
    foreach($placed_industry_list as $arr)
    {
        $placed_industry_count+=$arr['count'];
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$arr['count']."</td>");
        echo("<td>".$arr['industry_name']."</td>");
        echo("</tr>");
    }
    if ($placed - $placed_industry_count > 0)
    {
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".($placed - $placed_industry_count - $placed_transfer)."</td>");
        echo("<td>Unknown</td>");
        echo("</tr>");
    }

    echo("<tr><td colspan='3'>&nbsp;</td></tr>");
}

//Placement by area 
$placed_area_count=0;
if ($placed_area_list)
{
    echo("<tr>");
        echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        echo("<td align='right'>&nbsp;</td>");
        echo("<td><b>Placement by area:</b></td>");
    echo("</tr>");
    asort($placed_area_list);
    reset($placed_area_list);
    foreach($placed_area_list as $arr)
    {
        $placed_area_count+=$arr['count'];
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$arr['count']."</td>");
        echo("<td>".$arr['area_name']."</td>");
        echo("</tr>");
    }
    if ($placed - $placed_area_count > 0)
    {
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".($placed - $placed_area_count - $placed_transfer)."</td>");
        echo("<td>Unknown</td>");
        echo("</tr>");
    }

    echo("<tr><td colspan='3'>&nbsp;</td></tr>");
}

//Placement by Canadian region 
$placed_region_count=0;
if ($placed_region_list)
{
    echo("<tr>");
        echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        echo("<td align='right'>&nbsp;</td>");
        echo("<td><b>Placement by Canadian region:</b></td>");
    echo("</tr>");
    asort($placed_region_list);
    reset($placed_region_list);
    foreach($placed_region_list as $arr)
    {
        $placed_region_count+=$arr['count'];
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$arr['count']."</td>");
        echo("<td>".$arr['region_name']."</td>");
        echo("</tr>");
    }
    if ($placed_canada - $placed_region_count > 0)
    {
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".($placed_canada - $placed_region_count)."</td>");
        echo("<td>Unknown</td>");
        echo("</tr>");
    }
}
echo("</table>");

echo("</form>");
echo("</td></tr></table>");
?>
