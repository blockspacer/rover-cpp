import React, {useEffect, useState} from 'react';
import {Table} from "react-bootstrap";
import Card from "react-bootstrap/Card";

const SystemInfo = (props) => {
    const [sysInfo, setSystemInfo] = useState(null);

    const api = props.config.api;

    useEffect(() => {
        const id = setInterval(() => {
            api.request("systemMonitor").then(function (result) {
                setSystemInfo(result)

            });
        }, 1000);

        return () => {
            clearInterval(id)
        };
    }, []);

    const renderRow = (name, value) => {
        if (value) {
            return (
                <tr>
                    <td className="text-right">{name}</td>
                    <td>{value}</td>
                </tr>
            )
        }

        return null
    };


    if (sysInfo === null) {
        return 'Loading...'
    }

    return (
        <Card>
            <Card.Header>{props.title}</Card.Header>
            <Card.Body>
                <Card.Text>
                    <Table striped bordered hover>
                        <tbody>
                        {renderRow("platform", sysInfo.platform)}
                        {renderRow("osName", sysInfo.osName)}
                        {renderRow("cpu", sysInfo.cpuCount)}
                        {renderRow("mem", sysInfo.physicalMemory / 1024 / 1024 / 1024 + " Gb")}
                        {renderRow("cpu temp", sysInfo.cpuTemp)}
                        {renderRow("battery temp", sysInfo.batteryTemp)}
                        </tbody>
                    </Table>
                </Card.Text>
            </Card.Body>
        </Card>
    );
};

export default SystemInfo;