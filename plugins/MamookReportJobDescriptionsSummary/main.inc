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
 
/* Filename:  main.inc
   Description:  form that gets a time period from the user and sends it off to contacts_pdf_batch.inc to build a report

   Variables coming in from the search:
       $job_id_list:  comma-delimited string of the job_id's that have come back from a job descriptions search
       $employer_id_list:  comma-delimited string of the employer_id's that have come back from a company search
       $division_id_list:  comma-delimited string of company division id's  that have come back from a division search
       $origin:  origin of where we're calling the report.  this will determine what the sql query is
       $search_term:  term being searched on
       $search_year:  year being searched on
 */

// close the stuff already prepended to each page
echo '</td><td align="left"><img src="misc/images', $_SESSION['SESSION_colorscheme'], '/topbar_right.jpg" alt="" /></td></tr></table>';

// title of the report
switch ($origin)
{
    default:
    case 'job_descriptions_search':
        echo '<h3>Job Descriptions - Job Posting Report</h3>';
        break;
    case 'employer_company_search':
        echo '<h3>Employer Information - Job Posting Report</h3>';
        break;
    case 'employer_division_search':
        echo '<h3>Employer Information - Job Posting Report</h3>';
        break;
}

// differentiate between a job descriptions search on one term and one done on ALL terms of a year
if ($origin == 'job_descriptions_search' && $search_term == 'ALL') {
    $origin = 'job_descriptions_search_all';
}

echo '<form method="post" name="myform" action="', $PHP_SELF, '&amp;select=job_descriptions_summary_report">';
    switch ($origin)
    {
        default:
        case 'job_descriptions_search':
            echo '<input type="hidden" name="job_id_list" value="', $job_id_list, '" />';
            break;
        case 'employer_company_search':
            echo '<input type="hidden" name="employer_id_list" value="', $employer_id_list, '" />';
            break;
        case 'employer_division_search':
            echo '<input type="hidden" name="division_id_list" value="', $division_id_list, '" />';
            break;
    }
    echo '<input type="hidden" name="origin" value="', $origin, '" />';

    echo '<table class="row1" cellpadding="5" cellspacing="0" border="0">';

    echo '<tr><td colspan="2" align="left" class="tableheaders">Set a range of dates for the days when jobs were posted:</td></tr>';
    // range of dates for the day the jobs were posted
    echo '<tr>';
        echo '<td align="right">From:  ';
        echo '<b class="red">', ($origin != 'job_descriptions_search' ? '*' : '');
    echo '</b></td><td><input type="text" name="min_date" maxlength="40" size="20" />&nbsp;', popup('min_date', 'myform').'</td>';
    echo '</tr>';

    echo '<tr>';
        echo '<td align="right">To:  </td>';
        echo '<td><input type="text" name="max_date" maxlength="40" size="20" />&nbsp;', popup('max_date', 'myform');
        echo '<br /><font size="2">(defaults to current date if not specified)</font></td>';
    echo '</tr>';

    if ($origin != 'job_descriptions_search')
    {
        // get the range of terms that the jobs are posted for
        // grab the available terms from term table
        echo '<tr><td colspan="2" align="left" class="tableheaders">-Or- Set a range of terms that jobs are valid for:</td></tr>';
        $sql = "SELECT year_order, term_name
                FROM term
                ORDER BY year_order";
        $results = $GLOBALS['dbh']->Execute($sql);
        while ($row = $results->FetchRow())
        {
            $terms[$row['year_order']] = $row['term_name'];
        }
        $limit = $results->RecordCount();

        echo '<tr class="row1"><td align="right">Start Term:  </td>';
        echo '<td>';
            echo '<select name="start_term">';

            $current_term_id = getCurrentTermID();

            echo '<option value="" selected="selected">&nbsp;</option>';
            foreach ($terms as $term_id=>$term_name)
            {
                echo '<option value="', $term_id, '">', $term_name, '</option>';
            }
            echo '</select>';

            // only one choice makes sense coming from the job descriptions search:
            // the year they chose as a search parameter
            if ($origin == 'job_descriptions_search_all')
            {
                echo '<select name="start_year">';
                echo '<option value="" selected="selected">&nbsp;</option>';
                echo '<option value="', $search_year, '">', $search_year, '</option>';
                echo '</select>';
            }
            else
            {
                // grab all the years in job_info and display them in the select options
                $sql = "SELECT DISTINCT year
                        FROM job_info
                        WHERE year <> '0000'";
                $results = $GLOBALS['dbh']->Execute($sql);
                echo '<select name="start_year">';
                
                echo '<option value="" selected="selected">&nbsp;</option>';
                while ($row = $results->FetchRow()) {
                    echo '<option value="', $row['year'], '">', $row['year'], '</option>';
                }
                echo '</select>';
            }
            echo '</td></tr>';

            // do all of the above again for the End term
            echo '<tr class="row1"><td align="right">End Term:  </td>';
            echo '<td>';
            echo '<select name="end_term">';

            echo '<option value="" selected="selected">&nbsp;</option>';
            foreach ($terms as $term_id=>$term_name)
            {
                echo '<option value="', $term_id, '">', $term_name, '</option>';
            }
            echo '</select>';

            if ($origin == 'job_descriptions_search_all')
            {
                echo '<select name="end_year">';
                echo '<option value="" selected="selected">&nbsp;</option>';
                echo '<option value="', $search_year, '">', $search_year, '</option>';
                echo '</select>';
            }
            else
            {
                $results->Move(0);
                echo '<select name="end_year">';
                echo '<option value="" selected="selected">&nbsp;</option>';
                while ($row = $results->FetchRow()) {
                    echo '<option value="', $row['year'], '">', $row['year'], '</option>';
                }
                echo '</select>';
            }
        echo '</td></tr>';

        echo '<tr>';
            echo '<td colspan=2>&nbsp;</td>';
        echo '</tr>';
    }

    // report order
    echo '<tr>';
        echo '<td align="right">Order report by:  </td>';
        echo '<td>';
            echo '<select name="order">';
                echo '<option value="job_code" selected="selected">Job Code</option>';
                echo '<option value="contact_name">Contact Name</option>';
                echo '<option value="company_name">Company Name</option>';
                echo '<option value="status">Job Status</option>';
                echo '<option value="date_posted">Date Posted</option>';
                echo '<option value="int_date">Interview Date</option>';
                echo '<option value="host_dept">Host Department</option>';
            echo '</select>';
        echo '</td>';
    echo '</tr>';

    // show students applied?
    //echo '<tr><td colspan="2" align="left" class="tableheaders">Choose whether or not to show the applications for each job</td></tr>';
    echo '<tr>';
        //echo '<td align="left" colspan="2"><input type="checkbox" name="show_applied" value="true" checked="checked" />Show students who applied to each job</td>';
        echo '<td align="right"><input type="checkbox" name="show_applied" value="true" checked="checked" /></td>';
        echo '<td>Show students who applied to each job</td>';
    echo '</tr>';

    // show cancelled jobs?
    echo '<tr>';
        echo '<td align="right"><input type="checkbox" name="show_cancelled_jobs" value="true" checked="checked" /></td>';
        echo '<td>Show cancelled jobs</td>';
    echo '</tr>';

    echo '<tr><td colspan="2"><hr /></td></tr>';

    echo '<tr>';
        echo '<td colspan="2" align="center"><input type="submit" value="Continue" /></td>';
    echo '</tr>';
    echo '</table>';
echo '</form>';

?>
