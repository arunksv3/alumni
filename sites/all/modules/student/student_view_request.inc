<?php

function student_view_request($viewtype, $viewid) {
    
    $li1 = "";
    $li2 = "";    
    $output = "";
    $key = variable_get('IIUM_Encryption_Key', NULL);
    $viewid = (int) decrypt($viewid, $key);

    if ($viewtype == "interview"){
        $table = "student_interview_request";
        $title = "View Interview request";
    }else{
        $table = "student_resume_request";
        $title = "View Resume request";
    }

    $comp_titles = comp_titles();
    $result = student_request_result($viewid, $table);
    if ($viewtype == "interview") {
        $jobinfo = jobinfo($result['jobid']);
        $li1 = '<li><label>Company Name: </label><span>' . $result['company_name'] . '</span></li>                
                <li><label>Job ID: </label><span>' . $jobinfo['jobid'] . '</span></li>';
        $li2 = '<li><label>Salary Details: </label><span>' . $result['salary_details']. '</span></li>
                <li>
                    <div class="view-inter-form"><label>Interview From: </label><span>' . dateformat($result['interview_date_from']) . ' ' . $result['interview_time_from'] . '</span></div>
                    <div class="view-inter-to"><label>To: </label><span>' . dateformat($result['interview_date_to']) . ' ' . $result['interview_time_to'] . '</span></div></li>
                <li><h2>Interview Venue</h2></li>
                <li><label>Street: </label><span>' . $result['street'] . '</span></li>
                <li><label>City: </label><span>' . $result['city'] . '</span></li>
                <li><label>State: </label><span>' . get_state_name($result['state']) . '</span></li>
                <li><label>Country: </label><span>' . get_country_name($result['country']) . '</span></li>';        
    }

    $std_id = $result['student_id'];
    
    $stdinfoarr = stdinfo($std_id);
    $name = $stdinfoarr['name'];

    $output .= '<div class="view-interview-request">
                    <h1>'.$title.'</h1>
                    <p class="note">Note: This Request Expires in 24 hours of the delivery</p>    
                    <ul>                        
                        <li><label>From Email Address: </label><span>' . ($viewtype == "interview" ? $result['email_id'] : $result['your_email_id']) . '</span></li>
                        <li><label>'.($viewtype == "interview" ? 'Subject/Job Title' : 'Subject') .'</label><span>' . ($viewtype == "interview" ? $result['job_title'] : $result['subject']) . '</span></li>
                        '.$li1.'
                    </ul>    
                    <ul>
                        <li><h2>Message</h2></li>
                        <li>Dear ' . $name . ',</li>
                        <li><span>' . $result['message'] . '</span></li>
                        '.$li2.'
                        <li><h2>Contact Person Details</h2></li>
                        <li><label>Name:</label><span>' . $comp_titles[$result['federal_title']] . ' ' . $result['p_title'] . ' ' . $result['contactperson_fname'] . ' ' . $result['contactperson_lname'] . '</span></li>                        
                        <li><label>Designation: </label><span>' . ($viewtype == "interview" ? $result['designation']  : $result['contactperson_designation'] ) . '</span></li>
                        <li><label>Company: </label><span>' . ($viewtype == "interview" ? $result['company']  : $result['contactperson_company'] ) . '</span></li>
                        <li><label>Call: </label><span>' .($viewtype == "interview" ?  $result['contact_call'] :  $result['contactperson_call']) . '</span></li>
                        <li><label>Email Address: </label><span>'  .($viewtype == "interview" ?  $result['email'] :  $result['contactperson_email']) . '</span></li>
                    </ul>
                </div>';
    return $output;
}