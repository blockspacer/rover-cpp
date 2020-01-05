import React, {useEffect, useState} from 'react';

const Clocks = () => {
    const [curTime, setCurTime] = useState(null);

    useEffect(() => {
        const id = setInterval(() => {
            setCurTime(new Date().toLocaleString());
        }, 1000);

        return () => {
            clearInterval(id);
        };
    }, []);


    return (
        <div style={{color: 'green', fontWeight: 'bold'}}>{curTime}</div>
    );
};

export default Clocks;