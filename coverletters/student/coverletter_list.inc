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
 | Filename: coverletter_list.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: Displays a list of all the coverletters in the system for the   |
 | student.                                                                     |
 +------------------------------------------------------------------------------+

*/

 echo ("<h3>Cover Letters - View</h3>");

// :TODO: This load_order() function doesn't work as there's three forms in this
//        file that are named form_order. The name has to be unique for this to
//        work. In addition the forms are passing all these variables around in
//        its hidden fields very haphazardly. Need to get rid of variables that
//        shouldn't be passed in some cases. This is interfering with paging and
//        dynamic sorting. 

// :TODO: Fix up this file's pagination. It's inefficient.
?>

<script type='text/javascript' language='javascript'>
<!--javascript

function load_order(order)
{
    document.form_order.order_by.value=order;
    document.form_order.submit();
}
//-->
</script>

<?php

//Table for generic & template coverletters
$nextTermYear = getNextTermYear();
$show_term = ($show_term ? $show_term : $nextTermYear["term"]);
$show_year = ($show_year ? $show_year : $nextTermYear["year"]);
echo ("<h4>Generic / Template:</h4>");

$sql = "SELECT c.coverletter_id, DATE_FORMAT(c.last_modified, '%b %d, %Y %H:%i')  AS last_modified,
		c.type_id 
		FROM coverletter AS c 
		WHERE student_number = '".$student_number."' 
		AND (c.type_id='".TEMPLATE_COVERLETTER."' OR c.type_id='".GENERIC_COVERLETTER."')";

$result = $GLOBALS['dbh']->Execute( $sql );
$Num_Rows_Gen = $result->RecordCount();

if ($Num_Rows_Gen <= 0) {

	notify('You do not have a Generic or a Template coverletter.');
}

else {
 echo ("<form action='$PHP_SELF' method='post'>");
    	
    echo ("<table cellspacing='0' cellpadding='0' border='1'>");
	echo ("<tr>");
		echo ("<td>");
	            echo ("<table border='0' cellpadding='2'>");
			echo ("<tr>");
				echo ("<td class='rowgrey' align='center'>");
				echo ("&nbsp; &nbsp;");
				echo ("</td>");

				echo ("<td class='rowgrey' align='center'>");
				echo ("<b class='white'>Type</b>");
				echo ("</td>");

				echo ("<td class='rowgrey' align='center'>");
				echo ("<b class='white'>Last Modified</b>");
				echo ("</td>");
			echo ("</tr>");

	$rowclass=0;
    
	
    while($row =  $result->FetchRow()){

		$rowclass % 2 ? $class = "row0d" : $class = "row1d";

		if ($row["type_id"] == TEMPLATE_COVERLETTER) {
			$c_type = 'Template';
			$has_template = true;
		}
		elseif ($row["type_id"] == GENERIC_COVERLETTER) {
			$c_type = 'Generic';
			$has_generic = true;
		}
		
		echo ("<tr>");
			echo ("<td class='$class' align='center' valign='middle'>");
			echo ("&nbsp; <input name='coverletter_id' type='radio' class='$class' value='" . $row["coverletter_id"] . "' /> &nbsp;");
            echo ("</td>");
			echo ("<td class='$class' align='center' valign='middle'>");
			echo ("&nbsp; $c_type &nbsp;");
			echo ("</td>");
			echo ("<td class='$class' align='center' valign='middle'>");
			echo ("&nbsp; " . $row["last_modified"] . " &nbsp;");
			echo ("</td>");
		echo ("</tr>");

		$rowclass++;
	} // while loop

	echo ("</table>");
    echo("</td>");
	echo("</tr>");
	echo("</table>");
    echo ("<table border='0' cellpadding='3' cellspacing='2'>");
    echo ("<tr>");
    echo ("<td align='center'>");
    echo ("<input type='hidden' name='select' value='coverletter' />");
    echo ("\n<input type='hidden' name='show_term' value='". $show_term ."' />");
    echo ("\n<input type='hidden' name='show_year' value='". $show_year ."' />");
    echo ("<input type='submit' name='page' value='View HTML' />");
    echo ("</td>");
    echo ("<td align='center'>");
    echo ("<input type='submit' name='page' value='View PDF' />");
    echo ("</td>");
    echo ("<td align='center'>");
    echo ("<input type='submit' name='page' value='Edit' />");
    echo ("</td>");
    echo ("</tr>");
    echo ("</table>");
    echo ("<input type='hidden' name='PDF' value='1' />");
   
   echo(" </form>
            <br />");
}// else there are some rows returned


// table for job specific
                        
echo ("<h4>Job Application Specific:</h4>");

if($order_by)
{
    if($order_by == "last_modified")
    {
        $select_sql = "SELECT DATE_FORMAT(c.last_modified, '%b %d, %Y %H:%i') AS last_modified, i.job_id, i.job_code, 
                    c.coverletter_id, c.type_id, ec.company_name ";
    }
    else
    {
        $select_sql = "SELECT $order_by, DATE_FORMAT(c.last_modified, '%b %d, %Y %H:%i') AS last_modified, i.job_id, i.job_code,
                            c.coverletter_id, c.type_id, ec.company_name ";
    }
}
else
{
    $select_sql = "SELECT ec.company_name, i.job_code, DATE_FORMAT(c.last_modified, '%b %d, %Y %H:%i') AS last_modified, i.job_id,
                        c.coverletter_id, c.type_id ";
}	

$from_sql =" FROM applications AS a, job_info AS i, coverletter AS c
		LEFT JOIN employer_company AS ec ON ec.employer_id = i.employer_id
		WHERE i.job_id = a.job_id "
        . ($show_term != 'ALL' ? " AND i.term_id = '$show_term' ": " ") 
        . ($show_year != 'ALL' ? " AND i.year = '$show_year' ": " ") ."
		AND a.coverletter_id = c.coverletter_id 
		AND a.student_number = '$student_number' 
		AND c.type_id = '1'";
        
if($order_by)
{
    if($order_by == "last_modified")
    {
        $order_by = "c.last_modified";
    }

    if(($lastorder == $order_by) && ($data_flow == "ASC"))
    {
        $data_flow = "DESC";
    }
    elseif(($lastorder == $order_by) && ($data_flow == "DESC"))
    {
        $data_flow = "ASC";
    }
    elseif(!$data_flow)
    {
        $data_flow = "DESC";
    }
    
    if($order_by == "i.job_code")
    {
        $order_sql = "ORDER BY $order_by $data_flow ";
    }
    else
    {
        $order_sql = "ORDER BY $order_by $data_flow, i.job_code ";
    }
        
    if($order_by == "last_modified")
    {
        $order_by = "c.last_modified, i.job_code";
    }
}
else
{
    $order_sql = "ORDER BY ec.company_name";
    $order_by = "ec.company_name";
    $data_flow = "ASC";
}
$sql = $select_sql." ".$from_sql." ".$order_sql;

//LEFT JOIN job_info as i ON (i.job_id = c.job_id)

echo ("\n<form name='form_order' action='$PHP_SELF' method='post'>");
echo("<table border='0' cellpadding='2' cellspacing='0' class='row1'>");
    echo("<tr>");
        echo("<td align='left'>");
        echo ("\n<input type='hidden' name='select' value='student_coverletters' />");
echo ("\n<input type='hidden' name='page' value='View Cover Letters' />");
echo ("\n<input type='hidden' name='PDF' value='1' />");
echo ("\n<input type='hidden' name='order_by' value='$order_by' />");
echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
echo ("\n<input type='hidden' name='student_number' value='".$student_number."' />");
echo ("\n<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
echo ("\n<input type='hidden' name='job_page' value='". $job_page ."' />");


        echo ("Show applications for jobs:");

        echo ("<select name='show_term'>");
        echo ("<option value='ALL'". ($show_term == 'ALL' ? " selected='selected'" : "") .">ALL</option>");

        $tsql = "SELECT term_id, term_name
        FROM term
        ORDER BY term_id";
        $result = $GLOBALS['dbh']->Execute ($tsql);
        while ($row = $result->FetchRow()) {

            echo ("<option value='". $row["term_id"] ."'". ($show_term == $row["term_id"] ? " selected='selected'" : "") .">". $row["term_name"] ."</option>");
        }
        echo ("</select> &nbsp;");

        echo ("<select name='show_year'>");
        echo ("<option value='ALL'". ($show_year == 'ALL' ? " selected='selected'" : "") .">ALL</option>");

        $tsql = "SELECT MIN(year) AS min_year
        FROM job_info
        WHERE (status!='0' AND status!='7')
        AND year > 1960";
        $result = $GLOBALS['dbh']->Execute($tsql);
        $row = $result->FetchRow();

            for ($year = ($nextTermYear["year"] + 1); $year >= (int)$row["min_year"]; $year--) 
        {
            echo ("<option value='$year'". ($show_year == $year ? " selected='selected'" : "") .">". $year ."</option>");
        }
        echo ("</select>");

        echo ("</td>");
        echo ("<td align='left'>");
        echo ("<input type='submit' value='Go' />");
        echo ("</td>");
    echo ("</tr>");
//echo ("</form>");
$results = $GLOBALS['dbh']->Execute( $sql );
$num_jobs = $results->RecordCount();

if ($num_jobs <= 0) {

	notify('You do not have any job specific applications.If you would like to add a job specific cover letter, 
		please do so by clicking the "Apply to Job" button through Job Descriptions.');
}

else {
    if ($jobs_per_page == "") { $jobs_per_page = 20; }
    if ($jobs_per_page < 5) { $jobs_per_page = 5; }
    if ($job_page == '') { $job_page = 0; }
    
    if ($num_jobs > 20) {

        $num_pages = ceil($num_jobs / $jobs_per_page);

        $job_pos_start = $job_page * $jobs_per_page;
        $job_pos_end = $job_pos_start + $jobs_per_page;

        $sql .= " LIMIT $job_pos_start, $jobs_per_page";

        unset($PDF);

       // echo ("\n<form name='form_order' action='$PHP_SELF' method='post'>");
        echo ("\n<input type='hidden' name='select' value='student_coverletters' />");
        echo ("\n<input type='hidden' name='page' value='View Cover Letters' />");
        echo ("\n<input type='hidden' name='PDF' value='1' />");
        echo ("\n<input type='hidden' name='order_by' value='$order_by' />");
        echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
        echo ("\n<input type='hidden' name='student_number' value='".$student_number."' />");

        /* :EXPIRY: May 31 2005 - Patrick - Removed this because it breaks the form its supposed to be copying.
        echo ("\n<input type='hidden' name='show_term' value='". $show_term ."' />");
        echo ("\n<input type='hidden' name='show_year' value='". $show_year ."' />");
        */
        
        //echo ("\n<table border='0' class='row1' cellspacing='0' cellpadding='4'>");

        echo ("\n<tr><td colspan='2'><hr /></td></tr>");
        echo ("\n<tr>");
        echo ("\n<td valign='bottom' colspan='2'>");
        echo("\n<table border='0' cellpadding='2' cellspacing='0' class='row1'>");
        echo("\n<tr>");
        echo("\n<td align='left'>");
        $page_number = $job_page + 1;
        echo ($num_jobs . " cover letter" . (($num_jobs != 1) ? "s" : "") . " on " . $num_pages . " page" . (($num_pages > 1) ? "s" : "; "));
        echo ("\n</td>
        <td align='left'>");
        echo ("<input type='text' name='jobs_per_page' size='3' value='$jobs_per_page' /> cover letters per page");
        echo("</td>");
        echo ("\n</tr>");

        echo ("\n<tr>");
        echo ("\n<td>");
        echo ("\n<select name='job_page'>");

        for ($j = 0; $j < $num_pages; $j ++)
        {

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
            if ($order == 'term_year')
            {
                $from = substr($from, 0, 2) . ". " . substr($from, -4, 4);
                $to = substr($to, 0, 2) . ". " . substr($to, -4, 4);
            }
            elseif
                ($order == 'h.department_id')
                {
                    $from = getDepartmentName($from);
                    $to = getDepartmentName($to);
                }
            else
            {
                if (strlen($from) > 12) { $from = substr($from, 0, 12) . "..."; }
                if (strlen($to) > 12) { $to = substr($to, 0, 12) . "..."; }
            }

            echo ("\n<option value='$j'". (($j == $job_page) ? " selected='selected'" : "") .">");
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
        //echo ("\n</form>");
        echo ("\n</td>");
        echo ("\n</tr>");
        echo ("\n</table>");
        $table_close = 1;
        }
     
    if($table_close == 1)
    {
        unset($table_close);
    }
    else
    {
        echo ("\n</table>");
    }

  //  echo ("<form action='$PHP_SELF' method='post'>");
    echo ("<input type='hidden' name='select' value='coverletter' />");
	
    echo ("<table cellspacing='0' cellpadding='0' border='1'>");
	echo ("<tr>");
		echo ("<td>");
			echo ("<table border='0' cellpadding='2'>");
			echo ("<tr>");
				echo ("<td class='rowgrey' align='center'>");
				echo ("&nbsp; &nbsp;");
				echo ("</td>");
                         
				echo ("<td class='rowgrey' align='center'>");
				echo ("<a class='orderable' href='javascript:load_order(\"ec.company_name\")'><b class='white'>Company</b></a>");
				echo ("</td>");

				echo ("<td class='rowgrey' align='center'>");
				echo ("<a class='orderable' href='javascript:load_order(\"i.job_code\")'><b class='white'>Job Code</b></a>");
				echo ("</td>");

				echo ("<td class='rowgrey' align='center'>");
				echo ("<a class='orderable' href='javascript:load_order(\"c.type_id\")'><b class='white'>Type</b></a>");
				echo ("</td>");

				echo ("<td class='rowgrey' align='center'>");
				echo ("<a class='orderable' href='javascript:load_order(\"last_modified\")'><b class='white'>Last Modified</b></a>");
				echo ("</td>");
			echo ("</tr>");

	$rowclass=0;
    $result = $GLOBALS['dbh']->Execute( $sql );
	while ($row = $result->FetchRow()) {

		$rowclass % 2 ? $class = "row0d" : $class = "row1d";
		$JobHREF= $PHP_SELF . "&amp;select=view_job&amp;level1=job_id&amp;job_id=" . $row["job_id"] . "&amp;show_jobs=preferences&amp;show_future=";
		
		echo ("<tr>");
			echo ("<td class='$class' align='center' valign='middle'>");
			echo ("&nbsp; <input name='coverletter_id' type='radio' class='$class' value='". $row["coverletter_id"] ."' />&nbsp;");
			echo ("</td>");

			echo ("<td class='$class' align='center' valign='middle'>");
			echo ("&nbsp;". (($row["job_code"] == "" ) ? "" : "<a href='$JobHREF' class='blue'>" . $row["company_name"] . "</a>") ."&nbsp;");
			echo ("</td>");

			echo ("<td class='$class' align='center' valign='middle'>");
			echo ("&nbsp;". (($row["job_code"] == "" ) ? "" : "<a href='$JobHREF' class='blue'>" . $row["job_code"] . "</a>") ."&nbsp;");
			echo ("</td>");

			echo ("<td class='$class' align='center' valign='middle'>");
			echo ("&nbsp;". (($row["type_id"] == 1 ) ? "Specific" : "Unknown") ."&nbsp;");
			echo ("</td>");

			echo ("<td class='$class' align='center' valign='middle'>");
			echo ("&nbsp;". (($row["last_modified"] == "" ) ? "" : $row["last_modified"]) ."&nbsp;");
			echo ("</td>");

			echo ("<td class='$class' align='center' valign='middle'>");
		echo ("</tr>");

		$rowclass++;
	
	} // while loop

	echo ("</table>");
    echo ("</td>");
   echo ("</tr>");
   // echo ("</table>");
echo("</table>");
	echo ("<br />");

    echo ("<table border='0' cellpadding='3' cellspacing='2'>");
    echo ("<tr>");
    echo ("<td align='center'>");
    echo ("<input type='submit' name='page' value='View HTML' />");
    echo ("</td>");

    echo ("<td align='center'>");
    echo ("<input type='submit' name='page' value='View PDF' />");
    echo ("</td>");

    echo ("<td align='center'>");
    echo ("<input type='submit' name='page' value='Edit' />");
    echo ("</td>");
    echo ("</tr>");
    echo ("</table>");
    echo ("<input type='hidden' name='PDF' value='1' />");

    echo ("</form>");

} // else there are some rows returned


/*$Num_Rows is a variable in the above include that contains the number of rows returned
  so we use it here to determine if we want to show the following buttons
  To keep things nice, put the Hidden form values at the end, instead of right after the Form tag
 */



include ('misc/HTMLtoPDFInfo/HTMLtoPDFInfo.inc');
echo ("<table>");
echo ("<tr>");
echo ("<td>");
echo ("\n<form name='form_ord' action='$PHP_SELF' method='post'>");
echo ("\n<input type='hidden' name='select' value='student_coverletters' />");
echo ("\n<input type='hidden' name='page' value='View Cover Letters' />");
echo ("\n<input type='hidden' name='order_by' value='' />");
echo ("\n<input type='hidden' name='PDF' value='1' />");
echo ("\n<input type='hidden' name='lastorder' value='$order_by' />");
echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
echo ("\n<input type='hidden' name='student_number' value='".$student_number."' />");
echo ("\n<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
echo ("\n<input type='hidden' name='job_page' value='". $job_page ."' />");


/* :EXPIRY: May 31 2005 - Patrick - Removed this because it breaks the form its supposed to be copying.
echo ("\n<input type='hidden' name='show_term' value='". $show_term ."' />");
echo ("\n<input type='hidden' name='show_year' value='". $show_year ."' />");
*/

echo ("\n</form>");
echo ("</td>");
echo ("</tr>");
echo ("</table>");

?>
