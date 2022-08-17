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
 | Filename: interviewer.class.inc                                              |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: interviewer.class.inc

class interviewer
{
	var $job_id;
	var $job_code;
	var $interviewer_id;
	var $time_slots;

	function interviewer($job_id, $job_code, $interviewer_id, $time_slots)
	{
		$this->job_id = $job_id;
		$this->job_code = $job_code;
		$this->interviewer_id = $interviewer_id;
		$this->time_slots = $time_slots;
	}

	// NEW FUNCTION BELOW
	function reduce_time_slots()
	{
	   $i = 0;
	   while ($i < count($this->time_slots) - 1)
   	{
	      if ($this->time_slots[$i]->end == $this->time_slots[$i + 1]->start
      	    &&
   	       $this->time_slots[$i]->room_id == $this->time_slots[$i + 1]->room_id)
	      {
         	$this->time_slots[$i]->end = $this->time_slots[$i + 1]->end;
      	   $this->time_slots[$i]->time_id = array_merge((array) $this->time_slots[$i]->time_id, (array) $this->time_slots[$i + 1]->time_id);
   	      $dump = array_splice($this->time_slots, $i + 1, 1);
	      }
      	else
   	   {
	         $i++;
   	   }
	   }
	}
	// NEW FUNCTION ABOVE

	function objToString()
	{
		$output = "Object interviewer:(";
		$output .= "job_id: '" . $this->job_id . "'";
		$output .= ", job_code: '" . $this->job_code . "'";
		$output .= ", interviewer_id: '" . $this->interviewer_id . "'";
		$output .= ", time_slots: {";
		$slots = count($this->time_slots);
		$slotslessone = $slots - 1;
		for ($i = 0; $i < $slots; $i++)
		{
			$output .= "'";
			$output .= $this->time_slots[$i]->objToString();
			$output .= "'";
			if ($i < $slotslessone) { $output .= ", "; }
		}
		$output .= "}";
		$output .= ")";
		return $output;
	}
}

?>
