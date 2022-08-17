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
 | Filename: MamookReportJobDescriptionsSummary.class.php                       |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/


if (IN_MAMOOK !== true) {
    header("HTTP/1.0 404 Not Found");
    die();
}

class MamookReportJobDescriptionsSummary extends PluginInterface {

    function MamookReportJobDescriptionsSummary() {
        $this->plugin_version_number = '1.0';
        $this->plugin_author = 'University of Victoria';
        $this->plugin_name = 'Job Description Summary Report';
        // it's also a division search result and job search result plugin
        $this->plugin_type = 'company_search_results_plugin';
    }

    function init() {
        register_hook("mamook.traffic_control.select", $this, "handleTraffic");
        register_hook("mamook.company_search_results.button", $this, "getCompanyButtonAndForm");
        register_hook("mamook.division_search_results.button", $this, "getDivisionButtonAndForm");
        register_hook("mamook.job_descriptions_search_results.button", $this, "getJobButtonAndForm");
    }

    function handleTraffic($select) {
        global $auth;
        global $PHP_SELF;

        $plugin_absolute_path = $this->getPluginAbsolutePath();

        // temporary work around to local scoping of variables
        if (ini_get('register_globals')) {
            include('misc/compatibility.inc');
        }

        switch ($select) {
            case "job_descriptions_summary_form":
                if ($auth->userlevel == OFFICE || $auth->userlevel == TRAFFICDIRECTOR) {
                    include($plugin_absolute_path.'main.inc');
                    return true;
                }
                else {
                    include('misc/loguser.inc');
                    return null;
                }

            case "job_descriptions_summary_report":
                if ($auth->userlevel == OFFICE || $auth->userlevel == TRAFFICDIRECTOR) {
                    include($plugin_absolute_path.'job_descriptions_summary_report.inc');
                    return true;
                }
                else {
                    include('misc/loguser.inc');
                    return null;
                }
            default:
                return null;
        }
    }

    // $arr_params['employer_id_list'] is the $id_list
    // $arr_params['PHP_SELF'] is $PHP_SELF
    function getCompanyButtonAndForm($arr_params) {
        $arr_return['button'] = "<input type='button' value='Job Posting Report' onclick='javascript:document.job_posting_report_form.submit()' /> ";

        $string = ('<form method="post" name="job_posting_report_form" action="'.$arr_params['PHP_SELF'].'&amp;select=job_descriptions_summary_form">');
        $string .= ('<input type="hidden" name="employer_id_list" value="'.urlencode($arr_params['employer_id_list']).'" />');
        $string .= ('<input type="hidden" name="origin" value="employer_company_search" />');
        $string .= ('</form>');

        $arr_return['form'] = $string;

        return $arr_return;
    }

    // $arr_params['department_id_list'] is the $id_list
    // $arr_params['PHP_SELF'] is $PHP_SELF
    function getDivisionButtonAndForm($arr_params) {
        $arr_return['button'] = "<input type='button' value='Job Posting Report' onclick='javascript:document.job_posting_report_form.submit()' />"; 

        $string = ('<form method="post" name="job_posting_report_form" action="'.$arr_params['PHP_SELF'].'&amp;select=job_descriptions_summary_form">');
        $string .= ('<input type="hidden" name="division_id_list" value="'.urlencode($arr_params['department_id_list']).'" />');
        $string .= ('<input type="hidden" name="origin" value="employer_division_search" />');
        $string .= ('</form>');

        $arr_return['form'] = $string;

        return $arr_return;
    }
    
    // $arr_params['department_id_list'] is the $id_list
    // $arr_params['PHP_SELF'] is $PHP_SELF
    // $arr_params['search_job'] is search_job is an object from the job description search results
    function getJobButtonAndForm($arr_params) {
        $arr_return['button'] = "<input type='button' value='Job Posting Report' onclick='javascript:document.job_posting_report_form.submit()' />";

        $string = ('<form method="post" name="job_posting_report_form" action="'.$arr_params['PHP_SELF'].'&amp;select=job_descriptions_summary_form">');
        $string .= ('<input type="hidden" name="job_id_list" value="'.urlencode($arr_params['job_id_list']).'" />');
        $string .= ('<input type="hidden" name="origin" value="job_descriptions_search" />');
        $string .= ('<input type="hidden" name="search_term" value="'.trim($arr_params['search_job']->search_start_term).'" />');
        $string .= ('<input type="hidden" name="search_year" value="'.trim($arr_params['search_job']->search_start_year).'" />');
        $string .= ('</form>');

        $arr_return['form'] = $string;

        return $arr_return;
    }
}

?>
