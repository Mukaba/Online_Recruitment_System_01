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
| Filename: job_summary.php                                                    |
+------------------------------------------------------------------------------+
| Description:                                                                 |
+------------------------------------------------------------------------------+

*/

?>
</td><td align="left" valign="top"><img src="misc/images<?php echo $_SESSION['SESSION_colorscheme']; ?>/topbar_right.jpg" alt="" /></td></tr></table>
<?php

$department_id=$auth->department;
$department_name=getDepartmentName($department_id);
$dept_string=$department_id;

if ($placement_submit == 'Go')
{
    $term_id = $term;
} else {
    $NextTermYear = getNextTermYear();
    $term_id=$NextTermYear['term'];
    $year=$NextTermYear['year'];
}

echo("<table border='0' cellspacing='0' cellpadding='2' width='99%'><tr><td>");
echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='job_summary' />");
echo("<h3 align='left'>Job Summary for ".$department_name."</h3>");
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
    SELECT min(ji.year) AS year
    FROM job_info as ji
    INNER JOIN department_job_join as djj ON djj.job_id=ji.job_id
    WHERE ji.year != '0'
    AND ji.status != ".INT_ONLY."
    AND djj.department_id='".$department_id."'
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
echo("<td colspan='2' align='center'>");
echo("<input type='submit' name='placement_submit' value='Go' />");
echo("</td>");

echo("</tr></table>");

// This section contains all of the database queries used to determine the stats

// Jobs hosted by department
$sql_jobhost = "SELECT count(DISTINCT job_id) AS jobhost 
        FROM job_info
        WHERE term_id='".$term_id."' AND year='".$year."'
        AND department_id IN (".$dept_string.")
        AND admin_status <> '". CANCELLED_DELETED ."'
        AND status <> ".INT_ONLY;
$result_jobhost = $GLOBALS['dbh']->Execute($sql_jobhost);
$row_jobhost = $result_jobhost->FetchRow();
$jobhost = $row_jobhost['jobhost'];

// Job opportunities (jobs available to students in department)
$sql_job_list = "SELECT ji.job_id
        , al.area_id, al.area_name 
        , ec.company_type_id, ct.type_name
        , ed.industry_id, i.industry_name
        FROM job_info AS ji
        LEFT JOIN department_job_join AS djj ON djj.job_id = ji.job_id
        LEFT JOIN employer_company AS ec ON ec.employer_id=ji.employer_id
        LEFT JOIN company_type AS ct ON ct.type_id=ec.company_type_id
        LEFT JOIN country_list AS cl ON cl.country_id=ji.country_id
        LEFT JOIN area_list AS al ON al.area_id = cl.area_id
        LEFT JOIN employer_department AS ed ON ed.department_id = ji.employer_department_id
        LEFT JOIN industries AS i ON i.industry_id = ed.industry_id
        WHERE djj.department_id IN (". $dept_string .") 
        AND ji.term_id='".$term_id."' AND ji.year='".$year."'
        AND ji.admin_status <> '". CANCELLED_DELETED ."'
        AND ji.status <> '".INT_ONLY."'
        ";
$result_job_list = $GLOBALS['dbh']->Execute($sql_job_list);
$jobopp=0;
while($row_job_list = $result_job_list->FetchRow())
{
    $jobopp++;
    if ($row_job_list['company_type_id'] != 0)
    {
        $job_company_type_list[$row_job_list['company_type_id']]['company_type_name'] = $row_job_list['type_name'];
        $job_company_type_list[$row_job_list['company_type_id']]['count']++;
    }
    if ($row_job_list['industry_id'] != 0)
    {
        $job_industry_list[$row_job_list['industry_id']]['industry_name'] = $row_job_list['industry_name'];
        $job_industry_list[$row_job_list['industry_id']]['count']++;
    }
    if ($row_job_list['area_id'] != 0)
    {
        $job_area_list[$row_job_list['area_id']]['area_name'] = $row_job_list['area_name'];
        $job_area_list[$row_job_list['area_id']]['count']++;
    }
}

// Job opportunities by region 
$sql_job_region_list = "SELECT ji.job_id
        , ji.region_id, rl.region_name, pl.abbreviation
        FROM job_info AS ji
        LEFT JOIN department_job_join AS djj ON djj.job_id = ji.job_id
        LEFT JOIN provstate_list AS pl ON pl.provstate_id = ji.provstate_id
        LEFT JOIN region_list AS rl ON rl.region_id = ji.region_id
        WHERE djj.department_id IN (".$dept_string.")
        AND ji.term_id='".$term_id."' AND ji.year='".$year."'
        AND ji.admin_status <> '". CANCELLED_DELETED ."'
        AND ji.status <> '".INT_ONLY."'
        AND ji.country_id= '". CANADA ."'
        ";
$result_job_region_list = $GLOBALS['dbh']->Execute($sql_job_region_list);
$jobopp_canada=0;
while($row_job_region_list = $result_job_region_list->FetchRow())
{
    $jobopp_canada++;
    if ($row_job_region_list['region_id'] != 0)
    {
        $prov_region = $row_job_region_list['abbreviation']." - ".$row_job_region_list['region_name'];
        $job_region_list[$row_job_region_list['region_id']]['region_name'] = $prov_region;
        $job_region_list[$row_job_region_list['region_id']]['count']++;
    }
}

// Job opportunities by discipline
$sql_job_discipline_list = "SELECT dd.discipline_name
    , dd.discipline_id
    , d.department_code
    FROM department_disciplines AS dd
    LEFT JOIN department_discipline_join AS ddj ON ddj.discipline_id = dd.discipline_id
    LEFT JOIN job_discipline_join AS jdj ON 
        (jdj.discipline_id = ddj.discipline_id OR jdj.discipline_id = '".ALL_DISCIPLINES."')
    LEFT JOIN job_info AS ji ON ji.job_id = jdj.job_id
    LEFT JOIN department_job_join AS djj ON djj.job_id = ji.job_id
    LEFT JOIN department AS d ON d.department_id = ddj.department_id
    WHERE ddj.department_id IN (".$dept_string.")
    AND djj.department_id IN (".$dept_string.")
    AND ji.term_id='".$term_id."' AND ji.year='".$year."'
    AND ji.admin_status <> '". CANCELLED_DELETED ."'
    AND ji.status <> '".INT_ONLY."'
    ";
$result_job_discipline_list = $GLOBALS['dbh']->Execute($sql_job_discipline_list);
while($row_job_discipline_list = $result_job_discipline_list->FetchRow())
{
    if ($row_job_discipline_list['discipline_id'] != 0)
    {
        $department_discipline = $row_job_discipline_list['department_code']." - ".$row_job_discipline_list['discipline_name'];
        $job_discipline_list[$row_job_discipline_list['discipline_id']]['disc_name'] = $department_discipline;
        $job_discipline_list[$row_job_discipline_list['discipline_id']]['count']++;
    }
}

// The following section contains the HTML code to display the results of the report
echo("<table>");
echo("<tr>");
    echo("<td align='right'>".$jobhost."</td>");
    echo("<td>Jobs hosted</td>");
echo("</tr>");
echo("<tr>");
    echo("<td align='right'>".$jobopp."</td>");
    echo("<td>Job opportunities</td>");
echo("</tr>");
echo("</table>");
echo("<table>");
echo("<tr><td colspan='3'>&nbsp;</td></tr>");

// Opportunities for each job discipline
$job_discipline_count=0;
if ($job_discipline_list)
{
    echo("<tr>");
        echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        echo("<td align='right'>&nbsp;</td>");
        echo("<td><b>Opportunities for each job discipline:</b></td>");
    echo("</tr>");
    asort($job_discipline_list);
    reset($job_discipline_list);
    foreach($job_discipline_list as $arr)
    {
        $job_discipline_count+=$arr['count'];
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$arr['count']."</td>");
        echo("<td>".$arr['disc_name']."</td>");
        echo("</tr>");
    }

    echo("<tr><td colspan='3'>&nbsp;</td></tr>");
}

//Opportunities by company type
$job_company_type_count=0;
if ($job_company_type_list)
{
    echo("<tr>");
        echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        echo("<td align='right'>&nbsp;</td>");
        echo("<td><b>Opportunities by company type:</b></td>");
    echo("</tr>");
    asort($job_company_type_list);
    reset($job_company_type_list);
    foreach($job_company_type_list as $arr)
    {
        $job_company_type_count+=$arr['count'];
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$arr['count']."</td>");
        echo("<td>".$arr['company_type_name']."</td>");
        echo("</tr>");
    }
    if ($jobopp - $job_company_type_count > 0)
    {
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".($jobopp - $job_company_type_count)."</td>");
        echo("<td>Unknown</td>");
        echo("</tr>");
    }

    echo("<tr><td colspan='3'>&nbsp;</td></tr>");
}

//Opportunities by industry 
$job_industry_count=0;
if ($job_industry_list)
{
    echo("<tr>");
        echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        echo("<td align='right'>&nbsp;</td>");
        echo("<td><b>Opportunities by industry:</b></td>");
    echo("</tr>");
    asort($job_industry_list);
    reset($job_industry_list);
    foreach($job_industry_list as $arr)
    {
        $job_industry_count+=$arr['count'];
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$arr['count']."</td>");
        echo("<td>".$arr['industry_name']."</td>");
        echo("</tr>");
    }
    if ($jobopp - $job_industry_count > 0)
    {
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".($jobopp - $job_industry_count)."</td>");
        echo("<td>Unknown</td>");
        echo("</tr>");
    }

    echo("<tr><td colspan='3'>&nbsp;</td></tr>");
}

//Opportunities by area
$job_area_count=0;
if ($job_area_list)
{
    echo("<tr>");
        echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        echo("<td align='right'>&nbsp;</td>");
        echo("<td><b>Opportunities by area:</b></td>");
    echo("</tr>");
    asort($job_area_list);
    reset($job_area_list);
    foreach($job_area_list as $arr)
    {
        $job_area_count+=$arr['count'];
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$arr['count']."</td>");
        echo("<td>".$arr['area_name']."</td>");
        echo("</tr>");
    }
    if ($jobopp - $job_area_count > 0)
    {
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".($jobopp - $job_area_count)."</td>");
        echo("<td>Unknown</td>");
        echo("</tr>");
    }

    echo("<tr><td colspan='3'>&nbsp;</td></tr>");
}

//Opportunities by Canadian region
$job_region_count=0;
if ($job_region_list)
{
    echo("<tr>");
        echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        echo("<td align='right'>&nbsp;</td>");
        echo("<td><b>Opportunities by Canadian region:</b></td>");
    echo("</tr>");
    asort($job_region_list);
    reset($job_region_list);
    foreach($job_region_list as $arr)
    {
        $job_region_count+=$arr['count'];
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".$arr['count']."</td>");
        echo("<td>".$arr['region_name']."</td>");
        echo("</tr>");
    }
    if ($jobopp_canada - $job_region_count > 0)
    {
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("<td align='right'>".($jobopp_canada - $job_region_count)."</td>");
        echo("<td>Unknown</td>");
        echo("</tr>");
    }
}

echo("</table>");

echo("</form>");
echo("</td></tr></table>");
?>
