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
 | Filename: general_display.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file is used to show the user a list of students, along    |
 | with criteria to narrow down this list. This file also gives the user the    |
 | ability to set multiple flags at once for a large group of students.         |
 | Author: -CHaN                                                                |
 +------------------------------------------------------------------------------+

*/

?>

<script type='text/javascript' language='javascript'>
<!--javascript

function checkAll()
{
	for (var i = 0; i < document.form2.elements.length; i++)
	{
		var e = document.form2.elements[i];
		if ((e.name != 'allbox') && (e.type == 'checkbox'))
		{
			e.checked = document.form2.allbox.checked;
		}
	}
}

function CheckCheckAll()
{
	var TotalBoxes = 0;
	var TotalOn = 0;

	for (var i=0; i < document.form2.elements.length; i++)
	{
		var e = document.form2.elements[i];
		if ((e.name != 'allbox') && (e.type=='checkbox'))
		{
			TotalBoxes++;
			if (e.checked)
			{
				TotalOn++;
			}
		}
	}

	if (TotalBoxes == TotalOn)
	{
		document.form2.allbox.checked = true;
	}
	else
	{
		document.form2.allbox.checked = false;
	}
}

function updateFlags()
{
	var j = 0;

	for (var i = 0; i < document.form2.elements.length; i++)
	{
		var e = document.form2.elements[i];
		if ((e.name != 'allbox') && (e.type == 'checkbox'))
		{
			var index = document.form2.student_flag[document.form2.student_flag.selectedIndex].value;

			var flag = flag_values[index];

			e.checked = flag[j];

			j++;
		}
	}
    CheckCheckAll();
}


//-->
</script>

<?php

$sql = ("
    SELECT sf.student_flags_id, sf.description
    FROM student_flags as sf
    INNER JOIN department_student_flags AS dsf
    ON dsf.student_flags_id=sf.student_flags_id
    WHERE department_id='" . $auth->department . "' AND sf.student_flags_id != '".PLACED_FLAG."'
    AND sf.student_flags_id != '".ELIGIBLE_FLAG."'
    ORDER BY sf.description
    ");
$result = $GLOBALS['dbh']->Execute($sql);

$flag_array = array();

$i = 0;
while($flag_row = $result->FetchRow())
{
    $flag_array[$i]['student_flags_id'] = $flag_row['student_flags_id'];
    $flag_array[$i]['description'] = $flag_row['description'];
    $i++;
}

// The eligible placed flags are "special" student flags, in that they do not reside in the student_flags_join table. Rather, they reside in the eligible_placed
// table. This code originally assumes all student flags will have an entry in the student_flags table that provides a description of the flag,etc. The eligible
// flags are unique in that they are time dependent. You have to supply a term_id and year for an eligible flag.

// So we get the maximum student_flags_id. Throughout the code you'll see statements like if($student_flag <= $max_student_flags_id) . Everything below this
// max_student_flags_id are normal student flags. Everything above it are temporary eligible flags. 
if (!$max_student_flags_id)
{
    for($i=0;$i<sizeof($flag_array);$i++)
    {
        if ($i == 0)
        {
            $max_student_flags_id = $flag_array[$i]['student_flags_id'];
        }

        elseif ($max_student_flags_id < $flag_array[$i]['student_flags_id'])
        {
            $max_student_flags_id = $flag_array[$i]['student_flags_id'];
        }
    }
}

$term_sql = ("
    SELECT term_id, term_name
    FROM term
    ORDER BY year_order
    ");
$start_year = date("Y") - 1;
// Note: Make sure term_results is not overwritten. This query is used later in the code. 
$term_results = $GLOBALS['dbh']->Execute($term_sql);

$ep_array = array();

// We create eligible flags for all terms from last year to next year. All of this is kept in an array called ep_array[counter][fields]. This holds all the 
// information required to make a student eligible.
$counter = $max_student_flags_id + 1;
for($year=$start_year;$year<=$start_year+2;$year++)
{
    while ($term_row=$term_results->FetchRow())
    {
        $ep_array[$counter]['student_flags_id'] = ELIGIBLE_FLAG;
        $ep_array[$counter]['term_id'] = $term_row['term_id'];
        $ep_array[$counter]['term_name'] = $term_row['term_name'];
        $ep_array[$counter]['year'] = $year;

        $counter++;
    }
    $term_results->Move(0);
}

// student_id_list is a comma delimited list of record ids.
if ($student_id_list)
{
    $student_id_list = urldecode($student_id_list);
}

// User has selected a only a year or only a term. You need both to complete a search. Return an error message. 
if (($placed_year && !$placed_term) || (!$placed_year && $placed_term))
{
    $error = 1;
}

if (($eligible_year && !$eligible_term) || (!$eligible_year && $eligible_term))
{
    $error = 2;
}

if ($form_submitted && !$error)
{
	if (!$student_flag)
	{
		 // If they just came to this page, they won't have a student flag chosen yet, so choose a 
		 // default for them.

        $student_flag = $flag_array[0]['student_flags_id'];
	}
    
    if(is_string($checked_flags))
    {
        $checked_flags = unpackObject($checked_flags);
        $student_ids =  unpackObject($student_ids);
        $student_id_list =  urldecode($student_id_list);
    }
     
    if ($checked_flags)
    {
        $checked_flags = trim_array($checked_flags);
    }

	// Pull the students out of the database, along with the flags and parameters requested. 
	
    if ($first_time)
    {
        $sql = ("
            SELECT DISTINCT sd.record_id, s.student_number, s.last_name, s.first_name, sfj.student_flags_id 
            FROM student AS s, student_flags AS sf
            INNER JOIN student_department sd
            ON s.student_number=sd.student_number
            ");

        // These are flags that users are searching for.
        if (sizeof($checked_flags) > 0)
        {
            for ($i=0; $i < sizeof($checked_flags); $i++)
            {
                $x = $i - 1;

                // In the first iteration, we have to join with the student department table. 
                if ($i==0)
                {
                    $sql .= ("
                        INNER JOIN student_flags_join sfj".$i."
                        ON sd.record_id = sfj".$i.".record_id
                        ");
                }

                // Otherwise, join student_flags_join with the last student_flags_join table. 
                else 
                {
                    $sql .= ("
                        INNER JOIN student_flags_join sfj".$i."
                        ON sfj".$x.".record_id = sfj".$i.".record_id
                        ");
                }
            }
        }

        $sql .= ("
            LEFT JOIN student_flags_join AS sfj 
            ON (sfj.record_id=sd.record_id AND sf.student_flags_id=sfj.student_flags_id
            ");
        if ($student_flag <= $max_student_flags_id)
        {
            $sql .= ("
                AND sfj.student_flags_id='".$student_flag."')
                ");
        } 
        // Slight SQL query optimization using this if/elseif 
        if ($placed_term && $placed_year && $eligible_term && $eligible_year)
        {
            $sql .= (" 
                INNER JOIN eligible_placed placed_ep 
                ON sd.record_id = placed_ep.record_id 
                INNER JOIN eligible_placed eligible_ep 
                ON placed_ep.record_id = eligible_ep.record_id 
                ");
        }

        elseif ($placed_term && $placed_year)
        {
            $sql .= (" 
                INNER JOIN eligible_placed placed_ep 
                ON sd.record_id = placed_ep.record_id 
                ");
        }

        elseif ($eligible_term && $eligible_year)
        {
            $sql .= (" 
                INNER JOIN eligible_placed eligible_ep 
                ON sd.record_id = eligible_ep.record_id 
                ");
        }

        $sql .=(" 
            WHERE sd.department_id = '".$auth->department."'
            ");
        
        
        // Search for students with certain flags checked. 
        if (sizeof($checked_flags) > 0)
        {
            for ($i=0; $i < sizeof($checked_flags); $i++)
            {
                $sql .= ("
                    AND sfj".$i.".student_flags_id = '".$checked_flags[$i]."'
                    ");
            }
        }

        if ($student_flag <= $max_student_flags_id)
        {
            $sql .= ("
                AND sf.student_flags_id='".$student_flag."' 
                ");                       
        }

        if (trim($first_name))
        {
            $first_name = preg_replace("/%/","",$first_name);
            $sql .= ("
                AND s.first_name LIKE '" . addslashes(removeslashes(trim($first_name))) . "%'
                ");
        }
        
        if (trim($last_name))
        {
            $last_name = preg_replace("/%/","",$last_name);
            $sql .= ("
                AND s.last_name LIKE '" . addslashes(removeslashes(trim($last_name))) . "%'
                ");
        }

        if (($placed_term && $placed_year))
        {
            $sql .= ("
                AND placed_ep.student_flags_id = '".PLACED_FLAG."' AND placed_ep.year = '".$placed_year."' AND placed_ep.term_id = '".$placed_term."'
                ");
        }

        if (($eligible_term && $eligible_year))
        {
            $sql .= ("
                AND eligible_ep.student_flags_id = '".ELIGIBLE_FLAG."' AND eligible_ep.year = '".$eligible_year."' AND eligible_ep.term_id = '".$eligible_term."'
                ");
        }

        $sql .= (" 
            ORDER BY s.last_name, s.first_name	
            ");

        $first_time = 0;
    }// end if
    
    // If they came from advanced search, they already have a list of record_id's. Or they just requested for a list of students, and we have a list of record_ids. 
    elseif ($advanced_search || !$first_time)
    {
        if ($advanced_search && sizeof(department_groups($auth->department)) > 1)
        {
            notify("Note: You can only set flags for students in your department. All students not in your department have been removed from the list below.");
        }
        $sql = ("
                SELECT DISTINCT sd.record_id, s.last_name, s.student_number, s.first_name
                ");
        
        if ($student_flag <= $max_student_flags_id)
        {
            $sql .= ("
                , sfj.student_flags_id 
                ");
        }

        $sql .= ("
                FROM student AS s
                INNER JOIN student_department sd
                ON s.student_number=sd.student_number
                ");

        if ($student_flag <= $max_student_flags_id)
        {
            $sql .= ("
                LEFT JOIN student_flags_join sfj
                ON (sd.record_id = sfj.record_id)
                AND sfj.student_flags_id = '".$student_flag."'
                ");
        }

        $sql .= ("
                WHERE sd.record_id IN (" . $student_id_list .") AND sd.department_id = '".$auth->department."'
                ORDER BY s.last_name, s.first_name
                ");
    }
    $results = $GLOBALS['dbh']->Execute($sql);
    $count = $results->RecordCount();

    $student_id_list = NULL;
    // Copy results from SQL to an array. 

    $row = array();

    while ($temp_row = $results->FetchRow())
    {
        $row[] = $temp_row;
    }
    
    // variables for displaying pages.
    if ($per_page_max=='') { $per_page_max=50; }

    if ($per_page_max < 5) { $per_page_max = 5; }
    $pages = ceil($count / $per_page_max);

    if ($start_row == '') { $start_row = 0; }
    $first = $start_row + 1;
    $end = $start_row + $per_page_max;
    if ($end > $count) { $end = $count; }

    $student_ids = null;

    // Creating record id lists. 
    for ($i=0;$i<$count;$i++)
    {
        $student_id_list .= $row[$i]['record_id'].",";
        if ($i>=$start_row && $i<$end)
        {
            $student_ids .= $row[$i]['record_id'].",";
        }
    }

    // $student_id_list is a comma delimited list of all the record_ids
    $student_id_list = substr($student_id_list, 0, (strlen($student_id_list) - 1));
    // $student_ids is a comma delimited list of all the record_ids on the page being displayed. 
    $student_ids = substr($student_ids, 0, (strlen($student_ids) - 1));

    if ($count > 50)
    {
        
        // This is the form responsible for paging. 
        echo("<form action='".$PHP_SELF."' method='post'>");
            echo("<input type='hidden' name='select' value='set_student_flags' />");
            echo("<input type='hidden' name='start_row' value='".$start_row."' />");
            echo("<input type='hidden' name='student_id_list' value=\"".urlencode($student_id_list)."\" />");
            echo("<input type='hidden' name='advanced_search' value=\"".$advanced_search."\" />");
            echo("<input type='hidden' name='first_time' value=\"".$first_time."\" />");
            echo("<input type='hidden' name='form_submitted' value='true' />");

            echo("<table border='0' cellpadding='0' cellspacing='0' width='350'>");
                echo("<tr align='center'>");
                    echo("<td class='row1'>");
                        echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>");
                            echo("<tr class='row1'>");
                                echo("<td>".$count." student" . (($count != 1) ? "s" : "") . " on ".$pages." page" . (($pages > 1) ? "s" : "") . "<br /></td>");
                                echo("<td><input type='text' name='per_page_max' size='4' maxlength='4' value='".$per_page_max."' /> students per page.</td>");
                            echo("</tr>");
                            echo("<tr class='row1'>");
                                echo("<td>");
                                    echo("<select name='start_row'>");
                                        //$result = $GLOBALS['dbh']->Execute($student_sql);
                                        for ($i = 0, $row_num_start = 0; $i < $pages; $i++, $row_num_start += $per_page_max)
                                        {
                                            $row_num_end = $row_num_start + $per_page_max - 1;
                                            if ($row_num_end >= $count) 
                                            {
                                                $row_num_end = $count - 1;
                                            }

                                            $job_code_start = $row[$row_num_start]['last_name'];
                                            $job_code_end = $row[$row_num_end]['last_name'];

                                            if ($start_row == $row_num_start)
                                            {
                                                $selected = "selected='selected'";
                                            }
                                            echo("<option value='$row_num_start' ".$selected.">" . $job_code_start . " to " . $job_code_end . "</option>\n");
                                            $selected = "";
                                        }
                                    echo("</select>");
                                echo("</td>");
                                echo("<td align='right'>
                                <input type='submit' value='View' /><input type='hidden' name='advance' value='TRUE' />
                                     </td>");
                            echo("</tr>");
                        echo("</table>");
                    echo("</td>");
                echo("</tr>");
            echo("</table>");
        echo("</form>");
    }//End if

	//$result = $GLOBALS['dbh']->Execute($sql);

    // Build the flags array that the javascript is going to reference.
    
    //for($i=$start_row;$i<$end;$i++)
    //{
    //    $student_ids_included[] = $row[$i]["record_id"];
    //}

    // We now have an array containing all of the student_ids that we need to check,
    // and an array containing all of the flags that can be set.  We need to form an
    // array that holds the value of each flag for each student id.
    

    $str = ("var flag_values = new Array();");

    // This code below looks like it might be really slow for large amounts of data.  Look at more efficient methods
    // Note: After a few reconfigurations of this code, the current code remains the fastest dispite the O(n^2) complexity. 
    
    for ($i = 0; $i < sizeof($flag_array); $i++)
    {
        $str2 .= ("flag_values[" . $flag_array[$i]['student_flags_id'] . "] = Array(");

        for ($j = $start_row; $j < $end; $j++)
        {
            $flag_sql = ("
                SELECT DISTINCT record_id, student_flags_id
                FROM student_flags_join
                WHERE record_id='" . $row[$j]['record_id'] . "'
                AND student_flags_id='" . $flag_array[$i]['student_flags_id'] . "'
                ");
            $flag_result = $GLOBALS['dbh']->Execute($flag_sql);
            
            if (!$flag_result->RecordCount())
            {
                $str2 .= ("'',");
            }
            else
            {
                $str2 .= ("'checked',");
            }
        }
        
        // Clear out the trailing comma, and close the array call.
        $str2 = substr($str2, 0, (strlen($str2) - 1));
        $str2 .= (");");
    }

    // Create special eligible flag array for javascript. 
    $counter = $max_student_flags_id + 1;
    for ($i=$counter;$i<sizeof($ep_array)+$counter;$i++)
    {
        $str2 .= ("flag_values[".$i."] = Array(");
        for ($j=$start_row;$j<$end;$j++)
        {
            $flag_sql = ("
                SELECT DISTINCT record_id
                FROM eligible_placed
                WHERE record_id ='".$row[$j]['record_id']."'
                AND student_flags_id='".$ep_array[$i]['student_flags_id']."'
                AND term_id = '".$ep_array[$i]['term_id']."'
                AND year = '".$ep_array[$i]['year']."'
                ");
            $flag_result = $GLOBALS['dbh']->Execute($flag_sql);
            
            if(!$flag_result->RecordCount())
            {
                $str2 .= ("'',");
            }
            else
            {
                $str2 .= ("'checked',");
            }
        }

        $str2 = substr($str2, 0, (strlen($str2) - 1));
        $str2 .= (");");
    }
    

	if (!$count && !$advanced_search)
	{
		notify("There are no students in the database that match the criteria you have entered.");
	}

	else
	{
?>
		<script type='text/javascript' language='javascript'>
		<!--javascript

		
		// We now have the two strings, str and str2 that will be used to create our array
		// holding all of the flag values for each student id.  Let's parse them.
		

		eval("<?php echo($str); ?>");
		eval("<?php echo($str2); ?>");

		//-->
		</script>
		
		<?php

		echo("<form method='post' name='form2' action='" . $PHP_SELF . "&amp;select=set_student_flags'>");
		echo("<input type='hidden' name='level1' value='flags' />");
		echo("<input type='hidden' name='form_submitted' value='true' />");
	
		echo("<table cellspacing='0' cellpadding='0' border='1'>");
		echo("<tr>");
			echo("<td>");
	          	echo("<table border='0' cellpadding='2'>");
	              	echo("<tr>");
		                echo("<td class='rowgrey' align='center'>&nbsp;&nbsp;");
                            echo("<b class='white'>Student Name</b>&nbsp;&nbsp;");
                        echo("</td>");
		                echo("<td class='rowgrey' align='center'>&nbsp;&nbsp;");

                        echo("<select name='student_flag' onchange='updateFlags()'>");

                        for($i=0;$i<sizeof($flag_array);$i++)
                        {
                            echo("<option value='" . $flag_array[$i]['student_flags_id'] . "' ");
                            if ($student_flag == $flag_array[$i]['student_flags_id'])
                            {
                                echo(" selected='selected'");
                            }
                            echo(">" . $flag_array[$i]['description'] . "</option>");
                        }

                        // Generates the option to make a student eligible for a term within last year to next year. 

                        for ($i=$max_student_flags_id+1;$i<sizeof($ep_array)+$max_student_flags_id+1;$i++)
                        {
                            echo("<option value='".$i."'");
                            if ($student_flag == $i)
                            {
                                echo("selected='selected'");
                            }
                            echo (">Eligible " . $ep_array[$i]['term_name']." ".$ep_array[$i]['year']."</option>");
                        }

                        echo("</select>");	
                        
                        echo("&nbsp;&nbsp;</td>");
                    echo("</tr>");
	
                    $rowclass = 0;

                    $checked_flags = packObject($checked_flags);

                    for ($i=$start_row;$i<$end;$i++)
                    {
                        // The student_ids_included array is used to keep track of which student id's were including by the criteria, so we have
                        // an idea which students to set and unset flags for when told to do so by the user.
                        $href = ($PHP_SELF . "&amp;select=set_student_flags&amp;student_id_list=".urlencode($student_id_list)."&amp;level1=specific&amp;checked_flags="
                                .$checked_flags."&amp;student_ids=".packObject($student_ids)."&amp;student_num=".$row[$i]['student_number']); 
                        echo("<tr>");
                            echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>&nbsp;&nbsp;");
                            echo("<a href='".$href."'>".$row[$i]['last_name'].", ".$row[$i]['first_name'] . "&nbsp;</a></td>");
                            echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>&nbsp;&nbsp;");
                            echo("<input type='checkbox' class='" . (($rowclass) ? "row0d" : "row1d") . "' name='flag_on[]' value='" . $row[$i]["record_id"] . "' onclick='CheckCheckAll()' ");
                            // Check off boxes if a student was eligible for a given term and year. 
                            if ($student_flag > $max_student_flags_id)
                            {
                                $check_sql = ("
                                    SELECT DISTINCT record_id
                                    FROM eligible_placed
                                    WHERE term_id = '".$ep_array[$student_flag]['term_id']."'
                                    AND year = '".$ep_array[$student_flag]['year']."'
                                    AND student_flags_id = '".$ep_array[$student_flag]['student_flags_id']."'
                                    AND record_id = '".$row[$i]['record_id']."'
                                    ");
                             
                                $check_results = $GLOBALS['dbh']->Execute($check_sql);
                                if ($check_results->RecordCount() == 1)
                                {
                                    echo("checked='checked'");
                                }
                            }
                            elseif ($row["student_flags_id"] == $student_flag)
                            {
                                echo("checked='checked'");
                            }
                            echo("/></td>");
                        echo("</tr>");
                
                        $rowclass = (($rowclass) ? 0 : 1);
                    }
                    $checked_flags = unpackObject($checked_flags);
                    
                    echo("<tr>");
                        echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' colspan='1' align='center'>");
                        echo("<input type='hidden' name='form_submitted' value='1' />");
                        // student_ids is a list of student record ids that are currently displayed on the page.
                        echo("<input type='hidden' name='student_ids' value='" . packObject($student_ids) . "' />");
                        // student_id_list are all the students listed in all pages. 
                        echo("<input type='hidden' name='student_id_list' value='".urlencode($student_id_list)."' />");
                        echo("<input type='hidden' name='advanced_search' value='".$advanced_search."' />");
                        echo("<input type='hidden' name='first_time' value='".$first_time."' />");
                        // start_row is for paging
                        echo("<input type='hidden' name='start_row' value='".$start_row."' />");
                        // Passing associative array
                        for ($i=$max_student_flags_id+1;$i<sizeof($ep_array)+$max_student_flags_id+1;$i++)
                        {
                            echo("<input type='hidden' name='ep_array[".$i."][student_flags_id]' value='".ELIGIBLE_FLAG."' />");
                            echo("<input type='hidden' name='ep_array[".$i."][term_id]' value='".$ep_array[$i]['term_id']."' />");
                            echo("<input type='hidden' name='ep_array[".$i."][term_name]' value='".$ep_array[$i]['term_name']."' />");
                            echo("<input type='hidden' name='ep_array[".$i."][year]' value='".$ep_array[$i]['year']."' />");
                        }
                        echo("<input type='hidden' name='max_student_flags_id' value='".$max_student_flags_id."' />");
                        if ($pages > 1)
                        {
                            echo("<input type='submit' name='continue' value='Set Flag and Save for All Students on All Pages' /><br />");
                            echo("&nbsp;");
                            echo("<input type='submit' name='continue' value='Unset Flag and Save for All Students on All Pages' />");
                            echo("<br />");
                        }
                        echo("<input type='submit' name='continue' value='Save Changes' />");
                        echo("</td>");
                        echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>");
                        echo("<b class='black'>Select/Deselect All <br />on this page: </b>");
                        echo("<input type='checkbox' class='" . (($rowclass) ? "row0d" : "row1d") . "' name='allbox' onclick='checkAll()' />");
                        echo("</td>");
                    echo("</tr>");
                echo("</table>");
			echo("</td>");
		echo("</tr>");
        echo("</table>");
		echo("<table border='0'>");
		echo("<tr>");
			echo("<td>&nbsp;</td>");
		echo("</tr>");
		echo("</table>");
		echo("</form>");

        // This javascript below makes sure the the Select/Deselect All button is in the proper state when this page has loaded. 
        ?>
        
        <script type='text/javascript' language='javascript'>
        <!--javascript
            
            updateFlags();

        //-->
        </script>

        <?php
	} // End else
} // End if	

elseif ($form_submitted && $error)
{
    switch ($error)
    {
        case 1:
            error("When searching for placed students, you need to enter a term and a year.");
            break;
        case 2:
            error("When searching for eligible students, you need to enter a term and a year.");
            break;
        default:
            error("An error has occurred while searching for students.");
    }
}

echo("<form method='post' action='" . $PHP_SELF . "&amp;select=set_student_flags'>");
echo("<input type='hidden' name='level1' value='flags' />");
echo("<input type='hidden' name='form_submitted' value='true' />");
echo("<input type='hidden' name='first_time' value='1' />");

echo("<table border='0' cellpadding='0' cellspacing='0'>");
    echo("<tr align='center'>");
        echo("<td class='row1'>");
            echo("<table border='0' cellpadding='0' cellspacing='0' class='row1'>");
                echo("<tr align='center'>");
                	echo("<td colspan='2' align='center'>");
                        echo("Show students that match the criteria below:<br />(Leave any fields blank that you do not wished to be used in the search)");
                    echo("</td>");
                echo("</tr>");

                echo("<tr>");
                    echo("<td colspan='2'>&nbsp;</td>");
                echo("</tr>");

                echo("<tr>");
                    echo("<td colspan='2' align='center'>");
                        echo("First name starts with: ");
                        echo("<input type='text' name='first_name' size='10' value=\"" . stripslashes($first_name) . "\" />");
                    echo("</td>");
                echo("</tr>");

                echo("<tr>");
                    echo("<td colspan='2' align='center'>");
                        echo("Last name starts with: ");
                        echo("<input type='text' name='last_name' size='10' value=\"" . stripslashes($last_name) . "\" />");
                    echo("</td>");
                echo("</tr>");

                echo("<tr>");
                    echo("<td colspan='2' align='center'>");
                        echo("Department: ".getDepartmentName($auth->department));
                    echo("</td>");
                echo("</tr>");

                echo("<tr>");
                    echo("<td colspan='2'>&nbsp;</td>");
                echo("</tr>");

                echo("<tr>");
                    echo("<td colspan='2' align='center'>");
                    echo("Students with these flags checked:");
                    echo("</td>");
                echo("</tr>");

                echo("<tr>");
                    echo("<td colspan='2'>&nbsp;</td>");
                echo("</tr>");

                $newrow = 0;
        
                for($i=0;$i<sizeof($flag_array);$i++)
                {		
                    if (!$newrow)
                    {
                        echo("<tr align='left'>");
                    }
        
                    echo("<td>");
                    echo("<input type='checkbox' name='checked_flags[]' value='" . $flag_array[$i]["student_flags_id"] . "' ");

                    if (is_array($checked_flags) && in_array($flag_array[$i]["student_flags_id"], $checked_flags))
                    {
                        echo("checked='checked'");
                    }
                    // By default, we have the current flag checked when a user first comes to this screen.
                    if ($flag_array[$i]['student_flags_id'] == CURRENT_FLAG && !$form_submitted){
                        echo ("checked='checked'");
                    }
                    echo(" />");
                    echo($flag_array[$i]["description"]);
                    echo("</td>");
        
                    if ($newrow)
                    {
                        echo("</tr>");
                    }
        
                    $newrow = (($newrow) ? 0 : 1);
                }
        
                if ($newrow)
                {
                    echo("<td>&nbsp;</td>");
                    echo("</tr>");
                }
        
                echo("<tr>");
                    echo("<td colspan='2'>&nbsp;</td>");
                echo("</tr>");

                // Search for placed students. 
                echo("<tr>");
                    echo("<td colspan='2' align='center'>Placed for &nbsp;");

                    // Reuse the associative array from $term_sql. Reset the array point to the first position of the array. 
                    $term_results->Move(0);
                    echo("<select name='placed_term'>");
                    echo("<option value=''>&nbsp;</option>");
                    while ($term_row = $term_results->FetchRow())
                    {
                        echo("<option value='".$term_row['term_id']."'");
                        if ($placed_term == $term_row['term_id'])
                        {
                            echo(" selected='selected'");
                        }
                        echo(">".$term_row['term_name']."</option>");
                    }
                    echo("</select>");
                    echo("&nbsp;");
                    $year_sql = ("
                        SELECT MIN(year) AS year
                        FROM eligible_placed
                        WHERE year != '0' AND student_flags_id = '".PLACED_FLAG."'
                        ");
                    $start_year = $GLOBALS['dbh']->GetOne($year_sql);
                    
                    $year_sql = ("
                        SELECT MAX(year) AS year
                        FROM eligible_placed
                        WHERE year != '0' AND student_flags_id = '".PLACED_FLAG."'
                        ");
                    $end_year = $GLOBALS['dbh']->GetOne($year_sql);
                    echo("<select name='placed_year'>");
                    echo("<option value=''>&nbsp;</option>");
                    for ($i=(int)$end_year;$i>=(int)$start_year; $i--)
                    {
                        echo("<option value='".$i."'");
                        if ($placed_year == $i)
                        {
                            echo(" selected='selected'");
                        }
                        echo(">".$i."</option>");
                    }
                    echo("</select>");
                    echo("</td>");
                echo("</tr>");
                
                // Search for eligible students. We only allow people to search for students eligible a year ago to a year ahead. 
                echo("<tr>");
                    echo("<td colspan='2' align='center'>Eligible for &nbsp;");
                    
                    $term_results->Move(0);
                    echo("<select name='eligible_term'>");
                    echo("<option value=''>&nbsp;</option>");
                    while ($term_row = $term_results->FetchRow())
                    {
                        echo ("<option value='".$term_row['term_id']."'");
                        if($eligible_term == $term_row['term_id'])
                        {
                            echo(" selected='selected'");
                        }
                        echo(">".$term_row['term_name']."</option>");
                    }
                    echo("</select>");
                    echo("&nbsp;");

                    $year_sql = ("
                        SELECT MIN(year) AS year
                        FROM eligible_placed
                        WHERE year != '0' AND student_flags_id = '".ELIGIBLE_FLAG."'
                        ");
                    $start_year = $GLOBALS['dbh']->GetOne($year_sql);
                    
                    $year_sql = ("
                        SELECT MAX(year) AS year
                        FROM eligible_placed
                        WHERE year != '0' AND student_flags_id = '".ELIGIBLE_FLAG."'
                        ");
                    $end_year = $GLOBALS['dbh']->GetOne($year_sql);

                    echo("<select name='eligible_year'>");
                    echo("<option value=''>&nbsp;</option>");
                    for ($i=(int)$end_year; $i>=(int)$start_year;$i--)
                    {
                        echo("<option value='".$i."'");
                        if($eligible_year == $i)
                        {
                            echo(" selected='selected'");
                        }
                        echo(">".$i."</option>");
                    }
                    echo("</select>");
                    echo("</td>");
                echo("</tr>");

                echo("<tr>");
                    echo("<td colspan='2'><hr /></td>");
                echo("</tr>");

                echo("<tr>");
                    echo("<td colspan='2' align='center'>");
                    echo("<input type='submit' value='Go'/>");
                    echo("</td>");
                echo("</tr>");

            echo("</table>");
        echo("</td>");
	echo("</tr>");
echo("</table>");
echo("</form>");
?>
