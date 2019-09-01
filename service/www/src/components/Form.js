import React, {useState} from 'react';
import Form from "react-bootstrap/Form";
import Button from "react-bootstrap/Button";

const FormTest= (props) => {
    const [title, setTitle] = useState("test");

    return (
        <div>
            <div>{title}</div>
            <Form>
                <Form.Group>
                    <Form.Label>Имя:</Form.Label>
                    <Form.Control type="text" onChange={(event) => setTitle(event.target.value)} />
                </Form.Group>
                <Button variant="primary" onClick={() => setTitle("Hello")}>Send</Button>
            </Form>
        </div>
    )
};

export default FormTest;