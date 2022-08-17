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
 | Filename: room_slot.class.inc                                                |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: room_slot.class.inc

class room_slot
{
	var $start;		// start time (in minutes from 00:00)
	var $end;		// end time (in minutes from 00:00)
	var $job_id;	// job_id
	var $interviewer_id; // interviewer_id

	function room_slot($job_id, $interviewer_id, $start, $end)
	{
		$this->job_id = $job_id;
		$this->interviewer_id = $interviewer_id;
		$this->start = $start;
		$this->end = $end;
	}

	function objToString()
	{
		$output = "Object room_slot:(";
		$output .= "start: '" . m2h($this->start) . "'";
		$output .= ", end: '" . m2h($this->end) . "'";
		$output .= ", job_id: '" . $this->job_id . "'";
		$output .= ", interviewer_id: '" . $this->interviewer_id . "'";
		$output .= ")";
		return $output;
	}
}

?>
