<?php

function events_news_events_list($form, &$form_state){
    global $base_url;
    global $user;
    
     $header = array();
    $eventsQuery = db_select('events')
        ->fields('events') 
        ->condition('status',1,'=');
     $table_sort = $eventsQuery->extend('TableSort') // Add table sort extender.
        ->orderByHeader($header); // Add order by headers.

    $pager = $table_sort->extend('PagerDefault')
        ->orderBy('from_date', 'DESC')
        ->limit(10); // 10 rows per page.
    
    $eventsResult = $pager->execute();
    $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
    $organizedArr = organized(); 
    $output = '<div class="form-item form-type-item"><ul class="viewall-ne-list">';
    $i=0;
    foreach($eventsResult as $row){
        if($row->photo!='')
            $image = ' <img src="'.file_create_url($row->photo).'"/>';
        else
            $image = '<img src="'.file_create_url('public://uploads/profile_pictures/noimage.jpg').'"/>';
        $i++;
        if($i%2==0)
            $class = 'white';
        else 
            $class = 'grey';
        $id = encrypt($row->id,$decrypting_key);
        //<h2><span>Event Code</span>: '.$row->event_code.'</h2>
        // <div class="tipid">'.sprintf("EV-%04d", $row->id).'</div>
        $output .='<li class="'.$class.'"> 
            <div class="view-ne-left">'.$image.'</div>
            <div class="view-ne-right">
            <span class="date-time">'.date('d M Y',strtotime($row->from_date)).' (Time: '.$row->from_time.') To '.date('d M Y',strtotime($row->to_date)).' (Time: '.$row->to_time.') </span>
            <h1><a href="'.$base_url.'/event/'.$id.'">'.$row->title.'</a></h1>
            <h2></h2>            
            <h3><span>Organized By</span>: '.$organizedArr[$row->organized_by].'</h3>
            
            <p>'.substr($row->description,0,200).'</p> 
              <a href="'.$base_url.'/event/'.$id.'">Read More</a>
            </div></li>'; 
    }
      $output .= '</ul></div>';
    /*$form['events'] = array(
        '#type' => 'item', 
        '#title' => '<h2>Events</h2> '
    );*/
    
    $form['events']['list'] = array(
        '#type' => 'item',         
        '#markup' => $output,         
     ); 
    $form['pager'] = array('#markup' => theme('pager')); 
    return $form;
    
}