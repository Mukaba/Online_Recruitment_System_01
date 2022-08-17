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
 | Filename: results.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description: This is the template for displaying history search results.     |
 +------------------------------------------------------------------------------+

 //:TODO: Fix this page's pagination. It's inefficient.

*/
?>
<h3>Employer Information - Hiring History</h3>
<script type='text/javascript' language='javascript'>

<!--javascript

function load_order(order_by)
{
    document.form_order.order.value=order_by;
    document.form_order.submit();
}
//-->
</script>
<?php

    if($paged)
    {
        $company_department = unpackObject(($com_dep));
    }
    
	if (sizeof($company_department)<>0){
		foreach($company_department as $x){
			$arrIDs=explode(":",$x);
			if ($arrIDs[0]<>NULL){
				if ($employer_list<>NULL){
					$employer_list=$employer_list."','".$arrIDs[0];
				}else{
					$employer_list=$arrIDs[0];
				}
			}
			if ($arrIDs[1]<>NULL){
				if ($department_list<>NULL){
					$department_list=$department_list."','".$arrIDs[1];
				}else{
					$department_list=$arrIDs[1];
				}
			}
		}
	}
	
     
    if ($employer_list<>NULL){
        $employer_list="('".$employer_list."')";
    }
    if ($employer_id<>NULL){
        $employer_list="('".$employer_id."')";
  
    }
    if ($department_list<>NULL){
        $department_list="('".$department_list."')";
    }
    if ($department_id<>NULL){
        $department_list="('".$department_id."')";
  
    }
    if ($term_id<>NULL){
        $term_sql = " and h.term_id=".$term_id." ";
    }
    if ($year<>NULL){
        $year_sql = " and h.year=".$year." ";
    }

    if ($departments<>NULL){
        if ($departments=="group"){
            $department_in_str="('".implode("','",$departments_in_group)."')";
        }else{
            $department_in_str="('".$departments."')";
        }
        $department_sql = " and sd.department_id in ".$department_in_str." ";
        if ($departments=="uvic"){
            $department_sql=NULL;
        }
    }

    if ($employer_list<>NULL){
		
        if($order)
        {
            if($order == "term_year")
            {
                $select_sql = ("
                    SELECT DISTINCT CONCAT(t.term_name, ', ', h.year) as term_year
                    , CONCAT(s.first_name,' ', s.last_name) as full_name, h.department_id
                    , CONCAT(h.salary,'/', sp.salary_period_name) as salary,h.work_term_number,h.history_id
                    , h.company_name, h.term_id, h.year 
                    ");
            }
            elseif($order == "full_name")
            {
                $select_sql = ("
                    SELECT DISTINCT CONCAT(s.first_name,' ', s.last_name) as full_name
                    , CONCAT(t.term_name, ', ', h.year) as term_year, h.department_id
                    , CONCAT(h.salary,'/', sp.salary_period_name) as salary
                    , h.work_term_number,h.history_id, h.company_name 
                    ");
            }
            elseif($order == "sp.salary_period_name,h.salary")
            {
                $select_sql = ("
                    SELECT CONCAT(h.salary,'/', sp.salary_period_name) as salary
                    , CONCAT(s.first_name,' ', s.last_name) as full_name
                    , CONCAT(t.term_name, ', ', h.year) as term_year, h.department_id
                    , h.work_term_number,h.history_id, h.company_name 
                    ");
            }
            else
            {
                $select_sql = ("
                    SELECT DISTINCT $order, CONCAT(t.term_name, ', ', h.year) as term_year
                    , CONCAT(s.first_name,' ', s.last_name) as full_name, h.department_id
                    , CONCAT(h.salary,'/', sp.salary_period_name) as salary
                    , h.work_term_number,h.history_id, h.company_name 
                    ");
            }
        }
        else
        {
            $select_sql = ("
                SELECT DISTINCT CONCAT(t.term_name, ', ', h.year) as term_year
                , CONCAT(s.first_name,' ', s.last_name) as full_name, h.department_id
                , CONCAT(h.salary,'/', sp.salary_period_name) as salary
                , h.work_term_number,h.history_id, h.company_name, h.term_id, h.year 
                ");
        }

            
        
        $from_sql = (" 
            FROM history as h 
            LEFT JOIN student as s 
            ON s.student_number=h.student_number
            LEFT JOIN term as t
            ON h.term_id=t.term_id
            LEFT JOIN student_department as sd
            ON s.student_number=sd.student_number
            LEFT JOIN salary_period sp
            ON h.salary_period_id = sp.salary_period_id
            WHERE h.employer_id IN ".$employer_list.$department_sql.$term_sql.$year_sql."
            ");
		if ($department_list<>NULL)
        {
				$from_sql = ("
                    FROM history as h 
                    LEFT join student as s 
                    ON s.student_number=h.student_number
                    LEFT JOIN term as t
                    ON h.term_id=t.term_id
                    LEFT JOIN student_department as sd
                    ON s.student_number=sd.student_number
                    LEFT JOIN salary_period sp
                    ON h.salary_period_id = sp.salary_period_id
                    WHERE h.employer_id IN ".$employer_list."
                    AND h.company_department_id IN ".$department_list.$department_sql.$term_sql.$year_sql."
                    ");
		}

        
        /* TODO: Have to add ordering for salary */
        if($order)
        {
            if(($lastorder == $order) && ($data_flow == "ASC"))
            {
                $data_flow = "DESC";
            }
            elseif(($lastorder == $order) && ($data_flow == "DESC"))
            {
                $data_flow = "ASC";
            }
            elseif(!$data_flow)
            {
                $data_flow = "DESC";
            }
                
            if($order == "term_year")
            {
                $order_sql = "ORDER BY year $data_flow, t.year_order $data_flow";
            }
            else
            {
                $order_sql = "ORDER BY $order $data_flow";
            }
        }
        else
        {
            $order_sql = "ORDER BY year DESC, t.year_order DESC";
            $order = "term_year";
            $data_flow = "DESC";
        }

        $sql = $select_sql." ".$from_sql." ".$order_sql;

    }else{
        $sql=NULL;
        echo ("</p>");
        notify("Please Specify a Company");
    }

    if ($sql<>NULL){
        
        $com_dep = packObject(($company_department));
        
        $results = $GLOBALS['dbh']->Execute($sql);
        $num_jobs = $results->RecordCount();
        if ($num_jobs == 0) {
            notify("This employer does not have any hiring history records.\n");
        } else {
            echo("<h4>Choose a Placement($num_jobs):</h4>");
            if ($jobs_per_page == "") { $jobs_per_page = 20; }
            if ($jobs_per_page < 5) { $jobs_per_page = 5; }
            if ($job_page == '') { $job_page = 0; }


            // display the thingy if necessary
            if ($num_jobs > 20) {
 //converting from html to xhtml since xhtml validator is complaining brakets  
  $department_list=$department_id;
                $num_pages = ceil($num_jobs / $jobs_per_page);

                $job_pos_start = $job_page * $jobs_per_page;
                $job_pos_end = $job_pos_start + $jobs_per_page;

                $sql .= " LIMIT $job_pos_start, $jobs_per_page";
                unset($PDF);
                echo ("\n<form method='post' action='$PHP_SELF'>");
                echo ("\n<input type='hidden' name='select' value='$select' />");
                echo ("\n<input type='hidden' name='order' value='$order' />");
                echo ("\n<input type='hidden' name='employer_id' value='$employer_id' />");
                echo ("\n<input type='hidden' name='com_dep' value='$com_dep' />");
                echo ("\n<input type='hidden' name='year' value='$year' />");
                echo ("\n<input type='hidden' name='term_id' value='$term_id' />");
                echo ("\n<input type='hidden' name='department_id' value='" . $department_id . "' />");
                echo ("\n<input type='hidden' name='department_list' value='" . $department_list . "' />");
                echo ("\n<input type='hidden' name='departments' value='" . $departments . "' />");
                echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
                echo ("\n<input type='hidden' name='paged' value='1' />");
                echo ("\n<input type='hidden' name='Submit' value='View' />");
                echo ("\n<table border='0' class='row1' cellspacing='0' cellpadding='4'>");
                
                echo ("\n<tr>");
                echo ("\n<td valign='bottom'>");
                $page_number = $job_page + 1;
                echo ($num_jobs . " history record" . (($num_jobs != 1) ? "s" : "") . " on " . $num_pages . " page" . (($num_pages > 1) ? "s" : "").": ");
                echo ("\n</td>");
                echo ("\n<td valign='bottom'>");
                echo ("<input type='text' name='jobs_per_page' size='3' value='$jobs_per_page' /> history records per page");
                echo ("\n</td>");
                echo ("\n</tr>");

                echo ("\n<tr>");
                echo ("\n<td>");
                echo ("\n<select name='job_page'>");

                for ($j = 0; $j < $num_pages; $j ++) {

                    $from_pos = $j * $jobs_per_page;
                    $to_pos = $from_pos + $jobs_per_page - 1;
                    if ($to_pos >= $num_jobs) $to_pos = $num_jobs - 1;

                    $results->Move($from_pos);
                    $from_row = $results->FetchRow();
                    $from = $from_row[0];

                    $results->Move($to_pos);
                    $to_row = $results->FetchRow();
                    $to = $to_row[0];

                    // special cases to concatenate the strings
                    if ($order == 'term_year') {
                        $from = substr($from, 0, 2) . ". " . substr($from, -4, 4);
                        $to = substr($to, 0, 2) . ". " . substr($to, -4, 4);
                    }
                    elseif($order == 'h.department_id'){
                        $from = getDepartmentName($from);
                        $to = getDepartmentName($to);
                    } 
                    else {
                        if (strlen($from) > 12) { $from = substr($from, 0, 12) . "..."; }
                        if (strlen($to) > 12) { $to = substr($to, 0, 12) . "..."; }
                    }

                    echo ("\n<option value='$j'". (($j == $job_page) ? "selected='selected'" : "") .">");
                    echo ((($from == '') ? "n/a" : $from) ." to  ". (($to == '') ? "n/a" : $to));
                    echo ("\n</option>");
                }
                echo ("\n</select>");
                echo ("\n</td>");

                echo ("\n<td align='left'>");
                echo ("\n<input type='submit' value='View' />");
                echo ("\n</td>");
            echo ("\n</tr>");
            echo ("\n</table>");
            echo ("\n</form>");
            }
            
            $result = $GLOBALS['dbh']->Execute($sql);

            if ($result->RecordCount() != 0) {
                // arrows
                if ($num_jobs > 20) {
                    echo ("<table cellspacing='0' cellpadding='0' border='0'>");
                    echo ("<tr>");
                    echo ("<td align='left'>");

                    if ($job_page > 0) {
                        unset($PDF);
                        echo ("\n<form method='post' action='$PHP_SELF'>");
                        echo ("\n<input type='hidden' name='select' value='$select' />");
                        echo ("\n<input type='hidden' name='employer_id' value='$employer_id' />");
                        echo ("\n<input type='hidden' name='com_dep' value='$com_dep' />");
                        echo ("\n<input type='hidden' name='year' value='$year' />");
                        echo ("\n<input type='hidden' name='term_id' value='$term_id' />");
                        echo ("\n<input type='hidden' name='department_id' value='" . $department_id . "' />");
                        echo ("\n<input type='hidden' name='departments' value='" . $departments . "' />");
                        echo ("\n<input type='hidden' name='paged' value='1' />");
                        echo ("\n<input type='hidden' name='Submit' value='View' />");
                        echo ("<input type='hidden' name='order' value='$order' />");
                        echo ("<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
                        echo ("<input type='hidden' name='job_page' value='". ($job_page - 1) ."' />");
                        echo ("<input type='submit' value='<' />");
                        echo ("</form>");
                    }

                    else
                    {
                        echo ("&nbsp;");
                    }

                    echo ("</td>");
                    echo ("<td align='right'>");

                    if ($job_page < ($num_pages - 1)) {
                        echo ("\n<form method='post' action='$PHP_SELF'>");
                        echo ("\n<input type='hidden' name='select' value='$select' />");
                        echo ("\n<input type='hidden' name='order' value='$order' />");
                        echo ("\n<input type='hidden' name='employer_id' value='$employer_id' />");
                        echo ("\n<input type='hidden' name='com_dep' value='$com_dep' />");
                        echo ("\n<input type='hidden' name='year' value='$year' />");
                        echo ("\n<input type='hidden' name='term_id' value='$term_id' />");
                        echo ("\n<input type='hidden' name='department_id' value='" . $department_id . "' />");
                        echo ("\n<input type='hidden' name='departments' value='" . $departments . "' />");
                        echo ("\n<input type='hidden' name='paged' value='1' />");
                        echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
                        //echo ("\n<input type='hidden' name='PDF' value='1' />");
                        echo ("\n<input type='hidden' name='Submit' value='View' />");
                        echo ("<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
                        echo ("<input type='hidden' name='job_page' value='". ($job_page + 1) ."' />");
                        echo ("<input type='submit' value='>' />");
                        echo ("</form>");
                    }

                    else
                    {
                        echo ("&nbsp;");
                    }

                    echo ("</td>");
                    echo ("</tr>");

                    echo ("<tr>");
                    echo ("<td colspan='2'>");
                }
                
                //$href = $PHP_SELF ."&amp;select=$select&amp;jobs_per_page=$jobs_per_page&amp;job_page=$job_page&amp;employer_id=$employer_id&amp;department_id=$department_id&amp;Submit=View&amp;lastorder=$order&amp;data_flow=$data_flow&amp;com_dep=$com_dep&amp;paged=1&amp;order=";
                
                    echo ("
                            <form action='$PHP_SELF' method='post'>
                            <input type='hidden' name='select' value='history' />
                            <input type='hidden' name='employer_id' value='$employer_id' />
                            <input type='hidden' name='com_dep' value='$com_dep' />
                            <input type='hidden' name='year' value='$year' />
                            <input type='hidden' name='term_id' value='$term_id' />
                            <input type='hidden' name='department_id' value='" . $department_id . "' />
                            <input type='hidden' name='departments' value='" . $departments . "' />
                            <input type='hidden' name='fromPage' value='employer info' />
                            <table border='1' cellpadding='0' cellspacing='0'><tr><td>
                            <table border='0' cellpadding='2'>
                            <tr>
                            <td class='rowgrey' align='center'>&nbsp;</td>
                            <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"h.company_name\")'><b class='white'>&nbsp;Company Name&nbsp;</b></a></td>
                            <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"h.department_id\")'><b class='white'>&nbsp;Placement Department&nbsp;</b></a></td>
                            <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"full_name\")'><b class='white'>&nbsp;Student Name&nbsp;</b></a></td>
                            <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"term_year\")'><b class='white'>&nbsp;Term&nbsp;</b></a></td>
                            <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"h.work_term_number\")'><b class='white'>&nbsp;WT#&nbsp;</b></a></td>
                            <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"sp.salary_period_name,h.salary\")'><b class='white'>&nbsp;Salary&nbsp;</b></a></td>
                            </tr>
                            ");
                    
                    $checked="checked='checked'";
                    $rowclass=1;
                    $results = $GLOBALS['dbh']->Execute($sql);
                    while ($row=$results->FetchRow()){
                    ?>
                        <tr class='<?php echo(($rowclass % 2) ? "row0d" : "row1d");?>'>
                        <?php
                        if (in_array($row['department_id'],$departments_in_group)){
                            ?>
                                <td align='center'>&nbsp;<input type='radio' <?php echo $checked?> name='history_id' value='<?php echo $row['history_id']?>' onclick='submit()' />&nbsp;</td>
                        <?php
                        
                        }
                        else{
                            echo "<td>&nbsp;</td>";
                        }
                        
                    ?>
                        <td align='center'>&nbsp;<?php echo $row['company_name']?>&nbsp;</td>
                        <td align='center'>&nbsp;<?php echo getDepartmentName($row['department_id'])?>&nbsp;</td>
                        <td align='center'>&nbsp;<?php echo $row['full_name']?>&nbsp;</td>
                        <td align='center'>&nbsp;<?php echo $row['term_year']?>&nbsp;</td>
                        <td align='center'>&nbsp;<?php echo $row['work_term_number']?>&nbsp;</td>
                        <td align='center'>&nbsp;<?php echo $row['salary']?>&nbsp;</td>

                        </tr>

                        <?php
                        $checked=NULL;
                    $rowclass++;
                    }
                    ?>
                        <tr class='<?php echo(($rowclass % 2) ? "row0d" : "row1d");?>'>
                        <td colspan='7' align='center'><input type='submit' name='Submit' value='Submit' /></td>
                        </tr>

                        </table>
                       </td></tr></table>
                        </form>
                        <?php
            }
        }
    }
  //converting from html to xhtml since xhtml validator is complaining brakets  
 $department_list=$department_id;
echo("<form name='form_order' action='$PHP_SELF' method='post'>");
echo ("\n<input type='hidden' name='select' value='$select' />");
echo ("\n<input type='hidden' name='order' value='' />");
echo ("\n<input type='hidden' name='lastorder' value='$order' />");
echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
echo ("\n<input type='hidden' name='employer_id' value='$employer_id' />");
echo ("\n<input type='hidden' name='com_dep' value='$com_dep' />");
echo ("\n<input type='hidden' name='year' value='$year' />");
echo ("\n<input type='hidden' name='term_id' value='$term_id' />");
echo ("\n<input type='hidden' name='department_id' value='" . $department_id . "' />");
echo ("\n<input type='hidden' name='department_list' value='" . $department_list . "' />");
echo ("\n<input type='hidden' name='departments' value='" . $departments . "' />");
echo ("\n<input type='hidden' name='paged' value='1' />");
echo ("\n<input type='hidden' name='Submit' value='View' />");
echo("</form>");
?>
