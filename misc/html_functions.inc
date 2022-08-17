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
| Filename: html_functions.inc                                                 |
+------------------------------------------------------------------------------+
| Description: useful functions for manipulating html                          |
+------------------------------------------------------------------------------+

*/
include_once 'functional.inc';

// converts a hash to an html attributes string, eg:
// attributes_string(array("foo" => "bar", "baz" => "quux"))
//   == 'foo="bar" baz="quux"'
function attributes_string($attr) {
    $s = '';
    if (!is_null($attr) && sizeof($attr) > 0) {
        foreach ($attr as $key => $value) {
            $s .= " $key=\"" . htmlentities($value) . '"';
        }
    }
    return $s;
}

// create a table header from an arbitrary number of arguments
function table_header() {
    $count = func_num_args();
    $html = '';
    if ($count > 0) {
        $args = func_get_args();

        $html = '<tr class="rowgrey">';
        for ($i = 0; $i < $count; $i++) {
            $html .= '<td><b class="white">'.$args[$i].'</td>';
        }
        $html .= '</tr>';
    }
    return $html;
}

?>
