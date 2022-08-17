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
 | Filename: jobs_search.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: This is the main file that directs the appropriate show page    |
 | to the OFFICE. Used for applications so staff can search easier, this file   |
 | actually displays the search parameters at the top of the page, then has     |
 | the include for the rest of the page.                                        |
 +------------------------------------------------------------------------------+

*/


$nextTermYear = getNextTermYear();

?>

<table cellspacing="0" cellpadding="0" border="0" width='100%'>
   <tr>
	<td colspan='2'>
	<form method='post' action='<?php echo($PHP_SELF); ?>&amp;select=applications&amp;option=$option'>

    <table border="0" cellspacing="0" cellpadding="2" width='99%'>
	<?php
    
    //if $show_jobs is not set, then set it to the users dept and access the default settings (either GROUP or DEPT) of their drop-drop down menus for
    //proper drop-down menu display later
    if($show_jobs == "")
    {    
        $sql = ("
            SELECT pulldown_menu_group 
            FROM contact_internal 
            WHERE department_id=".$auth->department." AND (login_id='".$login_id."' OR netlink_id='".$login_id."')
            ");
        
        $results=$GLOBALS['dbh']->Execute($sql);
        $row=$results->FetchRow();
        
        if ($row['pulldown_menu_group'] == 1)
        {
            //GROUP default drop-down menu
            $show_jobs = 'group';
        }
        else
        {
            //DEPT default drop-down menu
            $show_jobs = 'department';
        }
    }
    
    $flag = 0;
    if($job_code)
    {
        if(isValidJobCode($job_code, ""))
        {
            $sql = "SELECT DISTINCT job_id ".
                "FROM job_info ".
                "WHERE job_code = '".$job_code."'";
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $job_id = $row["job_id"];
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                $level1 = "job_id";
            } else
            {
                include('misc/loguser.inc');
            }
        }
        else
        {
            $flag = 1;
            error("You must enter a valid Job Code");
        }
    }
    else
    {
        $flag =1;
    }

if($flag == 1)
{   
    if ($level1 != "job_id")
    {
        echo("<tr>");
        echo("<td>");
			echo("<h3 align='left'>Job Applications</h3>");
			echo("</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td>");
			echo("<table border='0' cellpadding='0' cellspacing='0' width='400'>");
			echo("<tr align='center'>");
			echo("<td class='row1'>");
//echo(" <form method='post' action='" . $PHP_SELF. "&amp;select=applications&amp;option=$option'>");
echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>"); 

			echo("<tr align='center'>");
			echo("<td align='right' width='60%'>");
			$show_closed = urldecode($show_closed);
			$show_jobs = urldecode($show_jobs);

			echo("<input type='hidden' name='show_closed' value='" . urlencode($show_closed) . "' />");
  			echo("Show jobs for:&nbsp;");
			echo("</td>");
			echo("<td align='left' colspan='4' width='40%'>");

			/*
			 Get term and year.
			*/

			$sql = ("
				SELECT DISTINCT term_id, term_name
				FROM term
				ORDER BY term_id
				");
			$term_result = $GLOBALS['dbh']->Execute($sql);
			
			$sql = ("
				SELECT MIN(year) AS min_year
				FROM job_info
				WHERE (status!='0' AND status!='".CURRENT_FLAG."')
				AND year > 1960
				");
			$year_result = $GLOBALS['dbh']->Execute($sql);
			$year_row = $year_result->FetchRow();
			$start_year = $year_row["min_year"];

			$sql = ("
				SELECT CURRENT_DATE
				");
			$year_result = $GLOBALS['dbh']->Execute($sql);
			$year_row = $year_result->FetchRow();
			
			$year_result = explode("-", $year_row["CURRENT_DATE"]);
			$year = $year_result[0];
	
			$limit = $year + 1;
			
			echo("<select name='show_term'>");
			echo("<option value='ALL' ");
			if ($show_term == 'ALL')
			{
				echo("selected='selected'");
			}
			echo(">ALL</option>");
			while ($term_row = $term_result->FetchRow())
			{
				echo("<option value='" . $term_row["term_id"] . "' ");
				if ($show_term == $term_row["term_id"] || (!$show_term && $term_row["term_id"] == $nextTermYear["term"]))
				{
					echo("selected='selected'");
				}
				echo(">" . $term_row["term_name"] . "</option>");
			}
			echo("</select>");
			echo("&nbsp;");
			echo("<select name='show_year'>");
			echo("<option value='ALL' ");
			if ($show_year == 'ALL')
			{
				echo("selected='selected'");
			}
			echo(">ALL</option>");
			for ($i = $limit; $i > (int)$start_year - 1; $i--)
			{
				echo("<option value='" . $i . "' ");
				if ($show_year == $i || (!$show_year && $i == $nextTermYear["year"]))
				{
					echo("selected='selected'");
				}
				echo(">" . $i . "</option>");
			}
			echo("</select>");
			echo("</td>");
		echo("</tr>");

		/*
		 Closed jobs?
		*/
/*
		echo("<tr align='center'>");
			echo("<td align='right'>");
			echo("Show closed jobs?&nbsp;");
			echo("</td>");
			echo("<td align='left' colspan='2'>");
			echo("<select name='show_closed'>");
			echo("<option value='Hide Closed Jobs' ");
			if ($show_closed == 'Hide Closed Jobs' || (!$show_closed && $option == "view"))
			{
				echo("selected='selected'");
			}
			echo(">No</option>");
			echo("<option value='Show Closed Jobs' ");
			if ($show_closed == 'Show Closed Jobs' || (!$show_closed && $option == "send"))
			{
				echo("selected='selected'");
				$show_closed = 'Show Closed Jobs';
			}
			echo(">Yes</option>");
			echo("</select>");
			echo("</td>");
		echo("</tr>");
*/$show_closed = 'Show Closed Jobs';
		/*
		 Match your department, group, or at UVic?
		*/
        
                                                 
	    echo("<tr align='center'>");
			echo("<td align='right' width='60%'>");
	                echo("Show jobs that:&nbsp;");
					echo("</td>");
					echo("<td align='left' colspan='4'>");
					echo("<select name='show_jobs'>");
					echo("<option value='department' ");
					if ($show_jobs == 'department')
					{
						echo("selected='selected'");
					}
					echo(">match your department</option>");
                    //Display 'co-op group' option only if user has a co-op group
                    if(sizeof($departments_in_group)>1)
                    {
					    echo("<option value='group' ");
					    if ($show_jobs == 'group')
					    {
						    echo("selected='selected'");
					    }
                
					    echo(">match your Co-op group</option>");
					}
                    // only display at UVic if viewing
						echo("<option value='uvic' ");
						if ($show_jobs == 'uvic')
						{
							echo("selected='selected'");
						}
						echo(">are registered at ".ORGANIZATION_CODE."</option>");
					echo ("<option value='host' ");
					if ($show_jobs == 'host') {
						echo ("selected='selected'");
					}
					echo (">your department hosts</option>");
					echo("</select>");
					echo("</td>");
					echo("</tr>");

					// do we want to view only certain jobs??
					echo("<tr align='center'>");
			echo("<td align='right' width='60%'>");
			echo("Application Method:&nbsp;");
			echo("</td>");
			echo("<td align='left' width='10%'>");
			echo("<input type='radio' class='row2' name='app_method' value='all' ");
				if (($app_method == 'all') || (!$app_method)) {
					echo("checked='checked'");
					$app_method = 'all';
				}
				echo(" />&nbsp;All ");
			echo("</td>");
			echo("<td align='left' width='10%' nowrap='nowrap'>");
			echo("<input type='radio' class='row2' name='app_method' value='".JOB_APP_METHOD_ONLINE."' ".
				(($app_method == JOB_APP_METHOD_ONLINE) ? "checked='checked'" : "") . " />&nbsp;Online ");
			echo("</td>");
			echo("<td align='left' width='10%' nowrap='nowrap'>");
			echo("<input type='radio' class='row2' name='app_method' value='".JOB_APP_METHOD_PAPER."' ".
				(($app_method == JOB_APP_METHOD_PAPER) ? "checked='checked'" : "") ." />&nbsp;Paper ");
			echo("</td>");
			echo("<td align='left' width='10%' nowrap='nowrap'>");
			echo("<input type='radio' class='row2' name='app_method' value='".JOB_APP_METHOD_SPECIAL."' ".
				(($app_method == JOB_APP_METHOD_SPECIAL) ? "checked='checked'" : "") ." />&nbsp;Special ");
			echo("</td>");
		echo("</tr>");
    
        //if $app_sent is not set, then set it to the desired defaults for sent/activated radio buttons for VIEW and SEND pages
        if($app_sent == "")
        {
            $sql = ("
                SELECT send_sent_apps 
                FROM contact_internal
                WHERE department_id='" .$auth->department. "'
                AND (login_id='" .$login_id. "' OR netlink_id='" .$login_id. "')
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            
            if($row = $result->FetchRow())
            {/* :EXPIRY: June 31 2005 - Patrick - Merging view/send pages
                if($option == 'view')
                {
                    switch($row['view_sent_apps'])
                    {
                        case '2':
                            $app_sent = 'all';
                            break;
                        case '1':
                            $app_sent = 'yes';
                            break;
                        case '0':
                            $app_sent = 'no';
                            break;
                        default:
                            assert(false);
                            break;
                    }
                }
                if($option == 'send')
                {*/
                    switch($row['send_sent_apps'])
                    {
                        case '2':
                            $app_sent = 'all';
                            break;
                        case '1':
                            $app_sent = 'yes';
                            break;
                        case '0':
                            $app_sent = 'no';
                            break;
                        default:
                            assert(false);
                            break;
                    }
                /*}*/
            }
            else
            {
                $app_sent ='no';
            }
        }

		echo("<tr align='center'>");
			echo("<td align='right' width='60%'>");
			echo("Application Sent / Activated:&nbsp;");
			echo("</td>");
            
			echo("<td align='left' nowrap='nowrap'>");
			echo("<input type='radio' class='row2' name='app_sent' value='all' ".
			    (($app_sent == 'all') ? "checked='checked'" : "") ." />&nbsp;All ");
			echo("</td>");
            
			echo("<td align='left'>");
			echo("<input type='radio' class='row2' name='app_sent' value='yes' ".
				(($app_sent == 'yes') ? "checked='checked'" : "") ." />&nbsp;Yes ");
			echo("</td>");
            
			echo("<td align='left'>");
			echo("<input type='radio' class='row2' name='app_sent' value='no' ".
				(($app_sent == 'no') ? "checked='checked'" : "") ." />&nbsp;No ");
			echo("</td>");
            
			echo("<td align='left'>&nbsp; </td>");
		echo("</tr>");

        echo("<tr align='center'>");
            echo("<td align='right' width='60%'>");
                echo("-OR- Enter Job Code &nbsp;");
            echo("</td>");
			echo("<td align='left' colspan='4'>");
                echo("<input type='text' value='' size='15' name='job_code' />");
            echo("</td>");
        echo("</tr>");	

		echo("<tr align='center'>");
			echo("<td colspan='5'><hr />");
			echo("<input type='submit' value='Go' />");
			
                       echo("</td>");
		echo("</tr>");

		echo("</table>");
	//	 echo("</form>");
        echo("</td>");
		echo("</tr>");
		echo("</table>");
		echo("</td>");
		echo("</tr>");
	}
	echo("<tr>");
	echo("<td>");
}

switch ($level1)
{
    case "job_id":
        if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
        {
            include('applications/admin/applicant_list.inc');
        } else
        {
            include('misc/loguser.inc');
        }
        break;

    default:
        if ($userlevel == OFFICE)
        {
            include("applications/admin/jobs_list.inc");
        } else
        {
            include("misc/loguser.inc");
        }
        break;
}


?>
	  </td>
        </tr>
      </table>
   </form> 
    </td>
  </tr>
</table>
