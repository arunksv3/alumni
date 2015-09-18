<?php 
function student_expiry_request() {
    global $user;
    $std_inter_expry_info = db_select('student_interview_request','sir')
        ->fields('sir', array('id', 'student_id', 'jobid', 'created_date'))  
        ->condition('status', '2', '=')
        ->execute()
        ->fetchAll();    
    
    foreach ($std_inter_expry_info as $value) {
        /* expiry after 24 hours of request */        
        $current_date = date('m-d-Y H:i:s'); 
        $nextday = date('m-d-Y H:i:s', strtotime($value->created_date . "+1 days"));  
        
        if($current_date >= $nextday){
            $sir_update = db_update('student_interview_request') 
                ->fields(array(              
                        'status' => '1',    
                    ))
                ->condition('id', $value->id, '=')
                ->execute();
            
            /* Maintain status histoty in user_status_history */
            if($sir_update){
                user_status_history('0', $value->student_id, $value->jobid, '1', 'student');
            }
        }
    }
    
    $std_resume_expry_info = db_select('student_resume_request', 'srr')
        ->fields('srr', array('id', 'student_id', 'created_date'))
        ->condition('status', '2', '=')
        ->execute()
        ->fetchAll();
    
    foreach ($std_resume_expry_info as $value) {
        /* expiry after 24 hours of request */        
        $current_date = date('m-d-Y H:i:s'); 
        $nextday = date('m-d-Y H:i:s', strtotime($value->created_date . "+1 days"));  
        
        if($current_date >= $nextday){
            $srr_update = db_update('student_resume_request') 
                ->fields(array(              
                        'status' => '1',    
                    ))
                ->condition('id', $value->id, '=')
                ->execute();
            
            /* Maintain status histoty in user_status_history */
            if($srr_update){
                user_status_history('0', $value->student_id, '0', '1', 'student');
            }
        }
    }
}